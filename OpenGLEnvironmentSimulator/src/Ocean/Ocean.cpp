
// Source:
// compute shaders and ssbo:
// registry.khronos.org/OpenGL/extensions/ARB/ARB_shader_storage_buffer_object.txt
// geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
// web.engr.oregonstate.edu/~mjb/cs557/Handouts/compute.shader.1pp.pdf

// Notes:
// 10/3: originally wanted to send data through ssbo as structs but gonna try
// as image/texture data
// 
// mPrime and nPrime are now values within range [0, M] and [0, N] respectively, 
// and are converted to proper ranges within functions when necessary

// Notes:
// lots of printing good for debugging - remove in release version
// add override keyword bum
// research better error handling practices? printOpenGLErrors() more
// fix order of member init lists EVERYWHERE, make everything const
// std::cerr instead of std::cout?
// organize shader folder
// refactor shadow class and model rendering function
// optimize compute shader - certain work group numbers better than others

// add more comments
// change pi

#include "Ocean.h"

const float Ocean::gravityConst = 9.81f;

void printVec3(glm::vec3 vec) {
	std::cout << vec.x << ", " << vec.y << ", " << vec.z << ", \n";
}

void printUVec3(glm::uvec3 vec) {
	std::cout << vec.x << ", " << vec.y << ", " << vec.z << ", \n";
}

// initializes vertices and indices vectors and creates ocean shader program
Ocean::Ocean(const uint32_t gridDimensions, const float waveHeight_A, glm::vec2 windDir_w, const float length, const LightData in_lightData)
	: m_OceanShaderProgram(ShaderProgram("./shaders/oceanVertShader.glsl", "./shaders/oceanFragShader.glsl")),
	  m_OceanComputeShader(ComputeShader("./shaders/oceanCompShader.glsl")),
	  m_GridVBO(0), m_GridVAO(0), m_GridEBO(0), m_GridSSBO(0), m_PositionAttrib(0), m_NormalAttrib(0),
	  m_GridSideDimension(gridDimensions), m_phillipsConstant_A(waveHeight_A), m_windDir_w(windDir_w), m_Length(length),
	  m_LightData(in_lightData)
{
	assert(m_GridSideDimension && !(m_GridSideDimension & (m_GridSideDimension - 1))); // m_GridSideDimension == power of 2
	
	std::cout << "[J] - Ocean Constructor \n";

	const uint32_t sidePlus1 = m_GridSideDimension + 1;  // one greater than grid dimension for tiling purposes
	const uint32_t verticesCount = sidePlus1 * sidePlus1;
	const uint32_t indicesCount = m_GridSideDimension * m_GridSideDimension * 2 * 3;

	m_Vertices = std::vector<OceanVertex>(verticesCount);
	m_Indices = std::vector<GLuint>(indicesCount);

	Initialize(); // generate grid data, bind VBO, VAO, and EBO objects

}

Ocean::~Ocean() 
{
	DeallocateResources();
}

// generate grid data, bind VBO, VAO, and EBO objects
void Ocean::Initialize() {

	// populate vertices
	std::complex<float> hTilde0, hTilde0mkConj;

	// note: for now, M and N are equal and so only represented as the member variable m_GridSideDimension
	uint32_t idx = 0;
	for (float m = 0; m <= m_GridSideDimension; ++m) {
		for (float n = 0; n <= m_GridSideDimension; ++n) {

			hTilde0 = hTilde_0(n, m);
			hTilde0mkConj = conj(hTilde_0(-n, -m));

			m_Vertices[idx].ox = static_cast<float>(n - m_GridSideDimension / 2.0f) * m_Length / m_GridSideDimension;
			m_Vertices[idx].oy = 0.0f;
			m_Vertices[idx].oz = static_cast<float>(m - m_GridSideDimension / 2.0f) * m_Length / m_GridSideDimension;

			m_Vertices[idx].x = m_Vertices[idx].ox;
			m_Vertices[idx].y = m_Vertices[idx].oy;
			m_Vertices[idx].z = m_Vertices[idx].oz;

			m_Vertices[idx].nx = 0.0f;
			m_Vertices[idx].ny = 1.0f;
			m_Vertices[idx].nz = 0.0f;

			m_Vertices[idx].a = hTilde0.real();
			m_Vertices[idx].b = hTilde0.imag();
			m_Vertices[idx].c = 0.0f;
			m_Vertices[idx]._a = hTilde0mkConj.real();
			m_Vertices[idx]._b = hTilde0mkConj.imag();
			m_Vertices[idx]._c = 0.0f;

			idx++;

		}
	}

	// populate indices with clockwise winding
	idx = 0;
	for (int m = 0; m < m_GridSideDimension; ++m) {
		int row1 = m * (m_GridSideDimension + 1);
		int row2 = (m + 1) * (m_GridSideDimension + 1);

		for (int n = 0; n < m_GridSideDimension; ++n) {
			// first triangle
			m_Indices[idx++] = row1 + n;
			m_Indices[idx++] = row1 + n + 1;
			m_Indices[idx++] = row2 + n + 1;

			// second triangle
			m_Indices[idx++] = row1 + n;
			m_Indices[idx++] = row2 + n + 1;
			m_Indices[idx++] = row2 + n;

		}
	}

	// Initialize vertex and fragment shader program
	m_OceanShaderProgram.UseProgram();
	m_PositionAttrib = glGetAttribLocation(m_OceanShaderProgram.getProgramID(), "inV_Pos");
	m_NormalAttrib = glGetAttribLocation(m_OceanShaderProgram.getProgramID(), "inV_Norm");

	// generate and bind grid VBO, VAO, and EBO - eventually extrapolate to VBO/EBO classes
	glGenBuffers(1, &m_GridVBO);
	glGenBuffers(1, &m_GridEBO);
	glGenVertexArrays(1, &m_GridVAO);

	glBindVertexArray(m_GridVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices[0]) * m_Vertices.size(), &m_Vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GridEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(m_PositionAttrib);
	glVertexAttribPointer(m_PositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (GLvoid*)0);

	GLintptr vertexPositionOffset = 8 * sizeof(GLfloat);
	glEnableVertexAttribArray(m_NormalAttrib);
	glVertexAttribPointer(m_NormalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (GLvoid*)vertexPositionOffset);

	// Compute shader **********

	m_OceanComputeShader.UseProgram();
	m_OceanComputeShader.SetInt("u_GridSideDimension", m_GridSideDimension);
	m_OceanComputeShader.SetFloat("u_Length", m_Length);
	m_OceanComputeShader.SetVec2("u_WindDirection", m_windDir_w);
	m_OceanComputeShader.SetFloat("u_PhillipsConstant", m_phillipsConstant_A);

	// Initialize shader storage buffer object (SSBO) //
	glGenBuffers(1, &m_GridSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GridSSBO);

	// copy data from m_Vertices into grid ssbo
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_Vertices[0]) * m_Vertices.size(), &m_Vertices[0], GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_GridSSBO);

	// **********

	printOpenGLErrors();
	std::cout << "[J] - Ocean object successfully initialized! \n\n";

}

void Ocean::Bind() {
	m_OceanComputeShader.UseProgram();
	m_OceanShaderProgram.UseProgram();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GridSSBO);
	glBindVertexArray(m_GridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GridEBO);

}

void Ocean::Render(const float time, glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_CamPos) { // render function

	// EvaluateWavesDFT(time); // CPU fourier evaluation - now done on GPU

	// Compute shader //
	m_OceanComputeShader.UseProgram();
	m_OceanComputeShader.SetFloat("u_Time", time);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GridSSBO);

	glDispatchCompute((m_GridSideDimension + 1) * (m_GridSideDimension + 1), 1, 1); // launches this many work groups
	glMemoryBarrier(GL_ALL_BARRIER_BITS); // makes sure data is computed before vertex shader is rendered

	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(m_Vertices[0]) * m_Vertices.size(), &m_Vertices[0]); // read SSBO data

	// Shader program //
	configureShaderProgram(in_ModelMat, in_ViewMat, in_ProjeMat, in_CamPos); // configure shader data

	glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(OceanVertex) * m_Vertices.size(), &m_Vertices[0]);
	glBindVertexArray(m_GridVAO);

	glDrawElements(GL_TRIANGLES, m_Indices.size() * 3, GL_UNSIGNED_INT, 0);

}

void Ocean::DeallocateResources() {

	// was breaking my whole program bruh
	glDeleteVertexArrays(1, &m_GridVBO);
	glDeleteBuffers(1, &m_GridVAO);
	glDeleteBuffers(1, &m_GridEBO);

}

void Ocean::configureShaderProgram(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_CamPos) {

	// Configure compute shader and update SSBO //
	//m_OceanComputeShader.UseProgram();
	//m_OceanComputeShader.SetFloat("u_Time", time);

	// Configure shader program //
	m_OceanShaderProgram.UseProgram();

	// render matrices
	m_OceanShaderProgram.SetMat4("u_Model", in_ModelMat);
	m_OceanShaderProgram.SetMat4("u_View", in_ViewMat);
	m_OceanShaderProgram.SetMat4("u_Projection", in_ProjeMat);

	// camera data
	m_OceanShaderProgram.SetVec3("u_CameraWorldPos", in_CamPos);

	// light data - shouldn't need to update each frame
	m_OceanShaderProgram.SetVec3("u_LightData.position", m_LightData.position);
	m_OceanShaderProgram.SetVec3("u_LightData.ambient", m_LightData.ambient);
	m_OceanShaderProgram.SetVec3("u_LightData.diffuse", m_LightData.diffuse);
	m_OceanShaderProgram.SetVec3("u_LightData.specular", m_LightData.specular);

	// other
	m_OceanShaderProgram.SetVec3("u_OceanColor", glm::vec3(0.0f, 0.2f, 0.2f));
	m_OceanShaderProgram.SetFloat("u_AmbientStrength", 0.25f);


}

// Tessendorf equations //

// Tessendorf equation 35
float Ocean::dispersionRelation(int32_t nPrime, int32_t mPrime) const {
	float w_0 = 2.0f * M_PI / 200.0f; // 200.0f = T (time after which cycle will repeat, Tessendorf equation 34)
	float kx = M_PI * (2 * nPrime - m_GridSideDimension) / m_Length;
	float kz = M_PI * (2 * mPrime - m_GridSideDimension) / m_Length;
	return floor(sqrt(gravityConst * sqrt(kx * kx + kz * kz)) / w_0) * w_0;

}

// Tessendorf equation 40/41
float Ocean::phillipsSpectrum(const int32_t mPrime, const int32_t nPrime) const {

	const float kx = M_PI * (2 * nPrime - m_GridSideDimension) / m_Length;
	const float ky = M_PI * (2 * mPrime - m_GridSideDimension) / m_Length;
	glm::vec2 k(kx, ky);

	const float kLen = glm::length(k);
	const float kLen2 = kLen * kLen;
	const float kLen4 = kLen2 * kLen2;
	if (kLen < 0.000001f) return 0.0f;

	const float kDotw = glm::dot(glm::normalize(k), glm::normalize(m_windDir_w));
	const float magnitudeKW = glm::length(kDotw);

	const float windSpeed_V = glm::length(m_windDir_w);

	const float L = (windSpeed_V * windSpeed_V) / gravityConst;
	const float kL2 = (L * L) * kLen2;

	float damping = 0.001f;
	float l2 = L * L * damping * damping; // from equation 41

	return m_phillipsConstant_A * (std::exp(-1.0f / (kL2)) / kLen4) 
		 * magnitudeKW * magnitudeKW * std::exp(-kLen2 * l2);

}

// Tessendorf equation 42, fourier amplitudes of wave height field
std::complex<float> Ocean::hTilde_0(int32_t nPrime, int32_t mPrime) const {

	// stackoverflow.com/questions/38244877/how-to-use-stdnormal-distribution
	std::random_device rd;
	std::mt19937 gen(rd());

	std::normal_distribution<float> gaussNumGen(0, 1); // mean = 0; standard dev = 1
	std::complex<float> randNum = gaussNumGen(gen);

	std::complex<float> res = randNum * sqrt(phillipsSpectrum(nPrime, mPrime) / 2.0f);
	return res;

}

// Tessendorf equation 43, fourier amplitudes of wave field realizations at time = t
std::complex<float> Ocean::hTilde(const float time, const int32_t nPrime, const int32_t mPrime) const {

	int index = mPrime * (m_GridSideDimension + 1) + nPrime;

	std::complex<float> hTilde0(m_Vertices[index].a, m_Vertices[index].b);
	std::complex<float> hTilde0Conj(m_Vertices[index]._a, m_Vertices[index]._b);

	float omegaT = dispersionRelation(nPrime, mPrime) * time;

	float cos_ = cos(omegaT);
	float sin_ = sin(omegaT);

	std::complex<float> c0(cos_, sin_);
	std::complex<float> c1(cos_, -sin_);

	std::complex<float> res = (hTilde0 * c0) + (hTilde0Conj * c1);
	return res;

}

heightDisplacementNormal Ocean::evalWaveData(glm::vec2 x, float t) {

	std::complex<float> height(0.0f, 0.0f);
	glm::vec2 displacement(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 0.0f, 0.0f);

	std::complex<float> c, hTilde_c, res;
	glm::vec2 k;
	float kx, kz, kLength, kDotx;

	for (float m = 0; m <= m_GridSideDimension; ++m) {
		kz = 2.0f * M_PI * (m - m_GridSideDimension / 2.0f) / m_Length;

		for (float n = 0; n <= m_GridSideDimension; ++n) {
			kx = 2.0f * M_PI * (n - m_GridSideDimension / 2.0f) / m_Length;
			k = glm::vec2(kx, kz);

			kLength = glm::length(k);
			kDotx = glm::dot(k, x);

			c = std::complex<float>(cos(kDotx), sin(kDotx));
			hTilde_c = hTilde(t, n, m) * c;

			height = height + hTilde_c;
			normal = normal + glm::vec3(-kx * hTilde_c.imag(), 0.0f, -kz * hTilde_c.imag());

			if (kLength >= 0.000001f) {
				displacement = displacement + glm::vec2(kx / kLength * hTilde_c.imag(), kz / kLength * hTilde_c.imag());
			}

		}

		normal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) - normal);
	}

	heightDisplacementNormal hdnRes;
	hdnRes.height = height;
	hdnRes.displacement = displacement;
	hdnRes.normal = normal;
	return hdnRes;

}

void Ocean::EvaluateWavesDFT(const float time) {

	float lambda = -1.0f;
	glm::vec2 x;
	glm::vec2 d;
	heightDisplacementNormal hdn;

	const int N = m_GridSideDimension;
	const int Nplus1 = m_GridSideDimension + 1;

	int index = 0;
	for (float m = 0; m < N; ++m) {
		for (float n = 0; n < N; ++n) {
			index = m * Nplus1 + n;
			
			x = glm::vec2(m_Vertices[index].x, m_Vertices[index].z);

			hdn = evalWaveData(x, time);

			m_Vertices[index].y = hdn.height.real();

			m_Vertices[index].x = m_Vertices[index].ox + lambda * hdn.displacement.x;
			m_Vertices[index].z = m_Vertices[index].oz + lambda * hdn.displacement.y;

			m_Vertices[index].nx = hdn.normal.x;
			m_Vertices[index].ny = hdn.normal.y;
			m_Vertices[index].nz = hdn.normal.z;

			
			if (n == 0 && m == 0) {
				m_Vertices[index + N + Nplus1 * N].y = hdn.height.real();

				m_Vertices[index + N + Nplus1 * N].x = m_Vertices[index + N + Nplus1 * N].ox + lambda * hdn.displacement.x;
				m_Vertices[index + N + Nplus1 * N].z = m_Vertices[index + N + Nplus1 * N].oz + lambda * hdn.displacement.y;

				m_Vertices[index + N + Nplus1 * N].nx = hdn.normal.x;
				m_Vertices[index + N + Nplus1 * N].ny = hdn.normal.y;
				m_Vertices[index + N + Nplus1 * N].nz = hdn.normal.z;
			}
			if (n == 0) {
				m_Vertices[index + N].y = hdn.height.real();

				m_Vertices[index + N].x = m_Vertices[index + N].ox + lambda * hdn.displacement.x;
				m_Vertices[index + N].z = m_Vertices[index + N].oz + lambda * hdn.displacement.y;

				m_Vertices[index + N].nx = hdn.normal.x;
				m_Vertices[index + N].ny = hdn.normal.y;
				m_Vertices[index + N].nz = hdn.normal.z;
			}
			if (m == 0) {
				m_Vertices[index + Nplus1 * N].y = hdn.height.real();

				m_Vertices[index + Nplus1 * N].x = m_Vertices[index + Nplus1 * N].ox + lambda * hdn.displacement.x;
				m_Vertices[index + Nplus1 * N].z = m_Vertices[index + Nplus1 * N].oz + lambda * hdn.displacement.y;

				m_Vertices[index + Nplus1 * N].nx = hdn.normal.x;
				m_Vertices[index + Nplus1 * N].ny = hdn.normal.y;
				m_Vertices[index + Nplus1 * N].nz = hdn.normal.z;
			}
			
		}
	}

}

// SSBO Code - in progress //

/* 
// generate and bind grid SSBO for compute shader
glGenBuffers(1, &m_GridSSBO);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_GridSSBO);
glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(m_Vertices[0]) * m_Vertices.size(), &m_Vertices[0], GL_DYNAMIC_DRAW);

// input ssbo data
struct OceanVertex* ssboPointer =
	(struct OceanVertex*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
		sizeof(m_Vertices[0]) * m_Vertices.size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

std::memcpy(ssboPointer, &m_Vertices[0], sizeof(m_Vertices[0]) * m_Vertices.size()); // *** use std::copy instead

// gets storage block index
GLuint blockIndex = glGetProgramResourceIndex(m_OceanComputeShader.getProgramID(), GL_SHADER_STORAGE_BLOCK, "ocean_data");
// connects shader storage block to SSBO
GLuint ssboBindingPointIdx = 0; // hardcoded in glsl *** not really sure if this is right lol
glShaderStorageBlockBinding(m_OceanComputeShader.getProgramID(), blockIndex, ssboBindingPointIdx);

GLenum currentError = glGetError();
if (currentError) std::cout << "[J] ERROR - OPENGL: " << currentError << std::endl;
*/

/* 10/5
	//GLvoid* ssbo_p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE); // retrieve pointer to buffer memory
	//std::memcpy(ssbo_p, &m_Vertices[0], sizeof(m_Vertices[0]) * m_Vertices.size()); // *** use std::copy instead

	// retrieve storage block index
	//GLuint ssboBlockIndex = glGetProgramResourceIndex(m_OceanComputeShader.getProgramID(), GL_SHADER_STORAGE_BLOCK, "ocean_data");
	//// connect block to SSBO
	//GLuint ssboBindingPointIndex = 0;
	//glShaderStorageBlockBinding(m_OceanComputeShader.getProgramID(), ssboBlockIndex, ssboBindingPointIndex);
	//// bind ssbo
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboBindingPointIndex, m_GridSSBO); // &&&
	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); // &&&
*/

