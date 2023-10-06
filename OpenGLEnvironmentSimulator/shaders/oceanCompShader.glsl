
// Ocean computer shader
// cg.tuwien.ac.at/courses/Realtime/repetitorium/VU.WS.2014/rtr_rep_2014_ComputeShader.pdf

// Notes:
// computergraphics.stackexchange.com/questions/3803/compute-shader-not-updating-buffer-or-vertex-buffer-unable-to-read-the-updates
// didnt finish adding the if statements form EvaluateWavesDFT

#version 430 core

#define PI 3.1415926538
#define GRAVITY 9.82

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in; // invocations per work group

struct ocean_data 
{
	// same as ocean vertex data in Ocean.h
	vec3 pos; // current position: x, y, z
	vec3 opos; // original position (for offset calc): ox, oy, oz
	vec3 norm; // normal: nx, ny, nz
	vec3 htilde; // htilde: a, b, c
	vec3 htilde0; // discrete fourier transform: _a, _b, _c

};

// std430 is the layout format, opengl 4.5 spec 7.6.2
layout(std430, binding = 0) buffer OceanData
{
	ocean_data gridVertices[];
	
};

uniform float u_Time;
uniform int u_GridSideDimension;
uniform float u_Length;
uniform vec2 u_WindDirection;
uniform float u_PhillipsConstant;

// heightDisplacementNormal struct for evalWaveData return type
struct heightDisplacementNormal {
	vec2 height_c;
	vec2 displacement;
	vec3 normal;

};


// Complex number functions (represented as vec2) //

vec2 multiply(vec2 a, vec2 b) {
	return vec2(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x);
}

// Tessendorf functions //

float dispersionRelation(int nPrime, int mPrime)  {
	float w_0 = 2.0f * PI / 200.0f; // 200.0f = T (time after which cycle will repeat, Tessendorf equation 34)
	float kx = PI * (2 * nPrime - u_GridSideDimension) / u_Length;
	float kz = PI * (2 * mPrime - u_GridSideDimension) / u_Length;
	return floor(sqrt(GRAVITY * sqrt(kx * kx + kz * kz)) / w_0) * w_0;

}

float phillipsSpectrum(int mPrime, int nPrime) {

	const float kx = PI * (2 * nPrime - u_GridSideDimension) / u_Length;
	const float ky = PI * (2 * mPrime - u_GridSideDimension) / u_Length;
	vec2 k = vec2(kx, ky);

	const float kLen = length(k);
	const float kLen2 = kLen * kLen;
	const float kLen4 = kLen2 * kLen2;
	if (kLen < 0.000001f) return 0.0f;

	const float kDotw = dot(normalize(k), normalize(u_WindDirection));
	const float magnitudeKW = length(kDotw);

	const float windSpeed_V = length(u_WindDirection);

	const float L = (windSpeed_V * windSpeed_V) / GRAVITY;
	const float kL2 = (L * L) * kLen2;

	float damping = 0.001f;
	float l2 = L * L * damping * damping; // from equation 41

	return u_PhillipsConstant * (exp(-1.0f / (kL2)) / kLen4) 
		 * magnitudeKW * magnitudeKW * exp(-kLen2 * l2);

}

vec2 hTilde(const int nPrime, const int mPrime) {

	// uint currentIndex = gl_GlobalInvocationID.x;
	uint currentIndex = mPrime * (u_GridSideDimension + 1) + nPrime;
	ocean_data vert = gridVertices[currentIndex];

	vec2 hTilde0_c = vec2(vert.htilde.x, vert.htilde.y);
	vec2 hTilde0Conj_c = vec2(vert.htilde0.x, vert.htilde0.y);

	float omegaT = dispersionRelation(nPrime, mPrime) * u_Time;

	float cos_ = cos(omegaT);
	float sin_ = sin(omegaT);

	vec2 c0_c = vec2(cos_, sin_);
	vec2 c1_c = vec2(cos_, -sin_);

	vec2 res_c = multiply(hTilde0_c, c0_c) + multiply(hTilde0Conj_c, c1_c);
	return res_c;

}

heightDisplacementNormal evalWaveData(const float in_xPos, const float in_zPos) {

	vec2 height_c = vec2(0.0f, 0.0f);

	vec2 displacement = vec2(0.0f, 0.0f);
	vec3 normal = vec3(0.0f, 0.0f, 0.0f);

	vec2 comp_c = vec2(0.0f, 0.0f);
	vec2 hTilde_c = vec2(0.0f, 0.0f);
	vec2 res_c = vec2(0.0f, 0.0f);

	vec2 k = vec2(0.0f, 0.0f);
	float kx = 0.0f;
	float kz = 0.0f;
	float kLength = 0.0f;
	float kDotx = 0.0f;

	for (int m = 0; m < u_GridSideDimension; m++) {
		kz = 2.0f * PI * (m - u_GridSideDimension / 2.0f) / u_Length;

		for (int n = 0; n < u_GridSideDimension; n++) {
			kx = 2.0f * PI * (n - u_GridSideDimension / 2.0f) / u_Length;
			k = vec2(kx, kz);

			kLength = length(k);
			kDotx = dot(k, vec2(in_xPos, in_zPos));

			comp_c = vec2(cos(kDotx), sin(kDotx));
			hTilde_c = multiply(hTilde(n, m), comp_c);

			height_c = height_c + hTilde_c;
			normal = normal + vec3(-kx * hTilde_c.y, 0.0f, -kz * hTilde_c.y);

			if (kLength >= 0.000001f) {
				displacement = displacement + vec2(kx / kLength * hTilde_c.y, kz / kLength * hTilde_c.y);
			}

		}

		normal = normalize(vec3(0.0f, 1.0f, 0.0f) - normal);

	}

	heightDisplacementNormal hdnRes;
	hdnRes.height_c = height_c;
	hdnRes.displacement = displacement;
	hdnRes.normal = normal;
	return hdnRes;

}


void EvaluateWaveDFT() {

	uint currentIndex = gl_GlobalInvocationID.x; // index works im ps
	ocean_data vert = gridVertices[currentIndex];

	// Evaluate wave position using discrete fourier transform //
	const float lambda = -1.0f;
	vec2 x = vec2(0.0f, 0.0f);
	vec2 d = vec2(0.0f, 0.0f);
	heightDisplacementNormal hdn;

	float xPos = vert.pos[0];
	float zPos = vert.pos[2];
	hdn = evalWaveData(xPos, zPos);

	vert.pos.y = hdn.height_c.x;

	vert.pos.x = vert.opos.x + (lambda * hdn.displacement.x);
	vert.pos.z = vert.opos.z + (lambda * hdn.displacement.y);

	vert.norm.x = hdn.normal.x;
	vert.norm.y = hdn.normal.y;
	vert.norm.z = hdn.normal.z;	

	gridVertices[currentIndex] = vert;

}

void main()
{

	EvaluateWaveDFT();

}

// position should be static here, but isnt
// check how data is being read and rendered, what data is being read from here

/*
uint currentIndex = gl_GlobalInvocationID.x; // index works im ps
ocean_data vert = gridVertices[currentIndex];

vert.pos.x = 1 * currentIndex;
vert.pos.y = 1 * currentIndex;
vert.pos.z = 1 * currentIndex;
	
gridVertices[currentIndex] = vert;
*/

/*
	bool m = currentIndex < u_GridSideDimension;
	bool n = currentIndex % u_GridSideDimension == 0;

	int N = u_GridSideDimension;
	int Nplus1 = N + 1;

	if (n && m) {
		gridVertices[currentIndex + N + Nplus1 * N].pos.y = hdn.height_c.x;

		gridVertices[currentIndex + N + Nplus1 * N].pos.x 
			= gridVertices[currentIndex + N + Nplus1 * N].opos.x + lambda * hdn.displacement.x;
		gridVertices[currentIndex + N + Nplus1 * N].pos.z 
			= gridVertices[currentIndex + N + Nplus1 * N].opos.z + lambda * hdn.displacement.y;

		gridVertices[currentIndex + N + Nplus1 * N].norm.x = hdn.normal.x;
		gridVertices[currentIndex + N + Nplus1 * N].norm.y = hdn.normal.y;
		gridVertices[currentIndex + N + Nplus1 * N].norm.z = hdn.normal.z;
	}
	if (n) {
		gridVertices[currentIndex + N].pos.y = hdn.height_c.y;

		gridVertices[currentIndex + N].pos.x 
			= gridVertices[currentIndex + N].opos.x + lambda * hdn.displacement.x;
		gridVertices[currentIndex + N].pos.z 
			= gridVertices[currentIndex + N].opos.z + lambda * hdn.displacement.y;

		gridVertices[currentIndex + N].norm.x = hdn.normal.x;
		gridVertices[currentIndex + N].norm.y = hdn.normal.y;
		gridVertices[currentIndex + N].norm.z = hdn.normal.z;
	}
	if (m) {
		gridVertices[currentIndex + Nplus1 * N].pos.y = hdn.height_c.y;

		gridVertices[currentIndex + Nplus1 * N].pos.x 
			= gridVertices[currentIndex + Nplus1 * N].opos.x + lambda * hdn.displacement.x;
		gridVertices[currentIndex + Nplus1 * N].pos.z 
			= gridVertices[currentIndex + Nplus1 * N].opos.z + lambda * hdn.displacement.y;

		gridVertices[currentIndex + Nplus1 * N].norm.x = hdn.normal.x;
		gridVertices[currentIndex + Nplus1 * N].norm.y = hdn.normal.y;
		gridVertices[currentIndex + Nplus1 * N].norm.z = hdn.normal.z;
	}
	*/