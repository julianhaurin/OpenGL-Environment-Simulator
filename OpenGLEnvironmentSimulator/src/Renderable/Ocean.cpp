
// Notes:
// phillips spectrum and dispersion functions both might not be accurate,
// other implementations have a 'length' parameter and use
// m - N / length for k vector, i think has to do with different for loop variables (i vs n_prime)
//
// wind speed is controlled by w vector (wind direction vector) 
// add length variable to adjust in-world size of ocean grid

// mPrime and nPrime are now values within range [0, M] and [0, N] respectively, 
// and are converted to proper ranges within functions when necessary

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
Ocean::Ocean(const uint32_t gridDimensions, const float waveHeight_A, glm::vec2 windDir_w, const float length)
	: m_GridSideDimension(gridDimensions), m_phillipsConstant_A(waveHeight_A), m_windDir_w(windDir_w), m_Length(length),
	  m_OceanShaderProgram(ShaderProgram("./shaders/oceanVertShader.glsl", "./shaders/oceanFragShader.glsl")),
	  // FFT
	  m_FFT(FFT(m_GridSideDimension)), 
	  m_HTilde(std::vector<std::complex<float>>(m_GridSideDimension * m_GridSideDimension)),
	  m_HTildeSlopeX(std::vector<std::complex<float>>(m_GridSideDimension * m_GridSideDimension)),
	  m_HTildeSlopeZ(std::vector<std::complex<float>>(m_GridSideDimension * m_GridSideDimension)),
	  m_HTildeDX(std::vector<std::complex<float>>(m_GridSideDimension * m_GridSideDimension)),
	  m_HTildeDZ(std::vector<std::complex<float>>(m_GridSideDimension * m_GridSideDimension)),
	  // Pocket FFT
	  m_PFFT_shape(static_cast<pocketfft::shape_t>(m_GridSideDimension)),
	  m_PFFT_stride(sizeof(float))
{
	assert(m_GridSideDimension && !(m_GridSideDimension & (m_GridSideDimension - 1))); // m_GridSideDimension == power of 2

	//std::cout << "Ocean Constructor \n";

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
			// m_Indices[idx++] = glm::uvec3(row1 + n, row1 + n + 1, row2 + n + 1);
			// m_Indices[idx++] = glm::uvec3(row1 + n, row2 + n + 1, row2 + n);

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

	m_OceanShaderProgram.UseProgram();
	m_PositionAttrib = glGetAttribLocation(m_OceanShaderProgram.getID(), "inV_Pos");
	m_NormalAttrib = glGetAttribLocation(m_OceanShaderProgram.getID(), "inV_Norm");

	//std::vector<float> vertices;
	//for (int i = 0; i < m_Vertices.size(); i++) {
	//	vertices.push_back(m_Vertices[i].x);
	//	vertices.push_back(m_Vertices[i].y);
	//	vertices.push_back(m_Vertices[i].z);
	//}
	//std::vector<uint32_t> indices;
	//for (int i = 0; i < m_Indices.size(); i++) {
	//	indices.push_back(m_Indices[i]);
	//}

	// generate and bind grid VBO, VAO, and EBO
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

	GLintptr vertexPositionOffset = 6 * sizeof(GLfloat);
	glEnableVertexAttribArray(m_NormalAttrib);
	glVertexAttribPointer(m_NormalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (GLvoid*)vertexPositionOffset);

	std::cout << "[J] - Ocean object successfully initialized! \n\n";
	
	/*
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_PositionAttrib);
	glVertexAttribPointer(m_PositionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	*/
	
	
}

void Ocean::Render(const float time, glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_LightPos) { // render function

	EvaluateWavesDFT(time);
	//EvaluateWavesFFT(time);

	m_OceanShaderProgram.UseProgram();
	m_OceanShaderProgram.SetMat4("u_Model", in_ModelMat);
	m_OceanShaderProgram.SetMat4("u_View", in_ViewMat);
	m_OceanShaderProgram.SetMat4("u_Projection", in_ProjeMat);

	m_OceanShaderProgram.SetFloat("u_AmbientStrength", 0.25f);
	m_OceanShaderProgram.SetVec3("u_OceanColor", glm::vec3(0.0f, 0.2f, 0.2f));
	m_OceanShaderProgram.SetVec3("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	m_OceanShaderProgram.SetVec3("u_LightPos", in_LightPos);
	
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(OceanVertex) * m_Vertices.size(), &m_Vertices[0]);
	glBindVertexArray(m_GridVAO);

	glEnableVertexAttribArray(m_PositionAttrib);
	glVertexAttribPointer(m_PositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (GLvoid*)0);
	GLintptr vertexPositionOffset = 6 * sizeof(GLfloat);
	glEnableVertexAttribArray(m_NormalAttrib);
	glVertexAttribPointer(m_NormalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(OceanVertex), (GLvoid*)vertexPositionOffset);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GridEBO);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 1 * 3);
	glDrawElements(GL_TRIANGLES, m_Indices.size() * 3, GL_UNSIGNED_INT, 0);

}

void Ocean::DeallocateResources() {

	//glDeleteVertexArrays(1, &m_GridVBO);
	//glDeleteBuffers(1, &m_GridVAO);
	//glDeleteBuffers(1, &m_GridEBO);

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
	if (kLen < 0.000001) return 0.0f;

	const float kDotw = glm::dot(glm::normalize(k), glm::normalize(m_windDir_w));
	const float magnitudeKW = glm::length(kDotw);

	const float windSpeed_V = glm::length(m_windDir_w);

	const float L = (windSpeed_V * windSpeed_V) / gravityConst;
	const float kL2 = (L * L) * kLen2;

	float damping = 0.001;
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

			if (kLength >= 0.000001) {
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

void Ocean::EvaluateWavesFFT(const float time) {

	float kx, kz, len, lambda = -1.0f;
	uint32_t index, index1;

	const uint32_t N = m_GridSideDimension;
	const uint32_t Nplus1 = N + 1;

	for (int m_prime = 0; m_prime < N; m_prime++) {
		kz = M_PI * (2.0f * m_prime - N) / m_Length;
		for (int n_prime = 0; n_prime < N; n_prime++) {
			kx = M_PI * (2 * n_prime - N) / m_Length;
			len = sqrt(kx * kx + kz * kz);
			index = m_prime * N + n_prime;

			m_HTilde[index] = hTilde(time, n_prime, m_prime);
			m_HTildeSlopeX[index] = m_HTilde[index] * std::complex<float>(0, kx);
			m_HTildeSlopeZ[index] = m_HTilde[index] * std::complex<float>(0, kz);
			if (len < 0.000001f) {
				m_HTildeDX[index] = std::complex<float>(0.0f, 0.0f);
				m_HTildeDZ[index] = std::complex<float>(0.0f, 0.0f);
			}
			else {
				m_HTildeDX[index] = m_HTilde[index] * std::complex<float>(0, -kx / len);
				m_HTildeDZ[index] = m_HTilde[index] * std::complex<float>(0, -kz / len);
			}
		}
	}

	//template<typename T> void c2c(
	//  const shape_t & shape, 
	//  const stride_t & stride_in,
	//	const stride_t & stride_out, 
	//  const shape_t & axes, 
	//  bool forward,
	//	const std::complex<T>*data_in, 
	//  std::complex<T>*data_out, 
	//  T fct,
	//	size_t nthreads = 1)

	for (int m_prime = 0; m_prime < N; m_prime++) {
		//m_PGFFT.apply(&m_HTilde[0], &m_HTilde[0]);
		//m_PGFFT.apply(&m_HTildeSlopeX[0], &m_HTildeSlopeX[0]);
		//m_PGFFT.apply(&m_HTildeSlopeZ[0], &m_HTildeSlopeZ[0]);
		//m_PGFFT.apply(&m_HTildeDX[0], &m_HTildeDX[0]);
		//m_PGFFT.apply(&m_HTildeDZ[0], &m_HTildeDZ[0]);

		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTilde[0], &m_HTilde[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeSlopeX[0], &m_HTildeSlopeX[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeSlopeZ[0], &m_HTildeSlopeZ[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeDX[0], &m_HTildeDX[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeDZ[0], &m_HTildeDZ[0], 1.0f, 1);

		//m_FFT.ComputeFFT(m_HTilde, m_HTilde, 1, m_prime * N);
		//m_FFT.ComputeFFT(m_HTildeSlopeX, m_HTildeSlopeX, 1, m_prime * N);
		//m_FFT.ComputeFFT(m_HTildeSlopeZ, m_HTildeSlopeZ, 1, m_prime * N);
		//m_FFT.ComputeFFT(m_HTildeDX, m_HTildeDX, 1, m_prime * N);
		//m_FFT.ComputeFFT(m_HTildeDZ, m_HTildeDZ, 1, m_prime * N);
	}
	for (int n_prime = 0; n_prime < N; n_prime++) {
		//m_PGFFT.apply(&m_HTilde[0], &m_HTilde[0]);
		//m_PGFFT.apply(&m_HTildeSlopeX[0], &m_HTildeSlopeX[0]);
		//m_PGFFT.apply(&m_HTildeSlopeZ[0], &m_HTildeSlopeZ[0]);
		//m_PGFFT.apply(&m_HTildeDX[0], &m_HTildeDX[0]);
		//m_PGFFT.apply(&m_HTildeDZ[0], &m_HTildeDZ[0]);

		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTilde[0], &m_HTilde[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeSlopeX[0], &m_HTildeSlopeX[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeSlopeZ[0], &m_HTildeSlopeZ[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeDX[0], &m_HTildeDX[0], 1.0f, 1);
		pocketfft::c2c(m_PFFT_shape, m_PFFT_stride, m_PFFT_stride, m_PFFT_shape, pocketfft::FORWARD, &m_HTildeDZ[0], &m_HTildeDZ[0], 1.0f, 1);

		//m_FFT.ComputeFFT(m_HTilde, m_HTilde, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeSlopeX, m_HTildeSlopeX, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeSlopeZ, m_HTildeSlopeZ, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeDX, m_HTildeDX, N, n_prime);
		//m_FFT.ComputeFFT(m_HTildeDZ, m_HTildeDZ, N, n_prime);
	}

	int sign;
	float signs[] = { 1.0f, -1.0f };
	glm::vec3 n;
	for (int m_prime = 0; m_prime < N; m_prime++) {
		for (int n_prime = 0; n_prime < N; n_prime++) {
			index = m_prime * N + n_prime;     // index into m_HTilde..
			index1 = m_prime * Nplus1 + n_prime;    // index into vertices

			sign = signs[(n_prime + m_prime) & 1];

			m_HTilde[index].real(m_HTilde[index].real() * sign);
			m_HTilde[index].imag(m_HTilde[index].imag() * sign);

			// height
			m_Vertices[index1].y = m_HTilde[index].real();

			// displacement
			m_HTildeDX[index].real(m_HTildeDX[index].real() * sign);
			m_HTildeDX[index].imag(m_HTildeDX[index].imag() * sign);
			m_HTildeDZ[index].real(m_HTildeDZ[index].real() * sign);
			m_HTildeDZ[index].imag(m_HTildeDZ[index].imag() * sign);

			m_Vertices[index1].x = m_Vertices[index1].ox + m_HTildeDX[index].real() * lambda;
			m_Vertices[index1].z = m_Vertices[index1].oz + m_HTildeDZ[index].real() * lambda;

			// normal
			m_HTildeSlopeX[index].real(m_HTildeSlopeX[index].real() * sign);
			m_HTildeSlopeX[index].imag(m_HTildeSlopeX[index].imag() * sign);
			m_HTildeSlopeZ[index].real(m_HTildeSlopeZ[index].real() * sign);
			m_HTildeSlopeZ[index].imag(m_HTildeSlopeZ[index].imag() * sign);

			n = glm::normalize(glm::vec3(0.0f - m_HTildeSlopeX[index].real(), 1.0f, 0.0f - m_HTildeSlopeZ[index].real()));
			m_Vertices[index1].nx = n.x;
			m_Vertices[index1].ny = n.y;
			m_Vertices[index1].nz = n.z;

			// for tiling
			if (n_prime == 0 && m_prime == 0) {
				m_Vertices[index1 + N + Nplus1 * N].y = m_HTilde[index].real();

				m_Vertices[index1 + N + Nplus1 * N].x = m_Vertices[index1 + N + Nplus1 * N].ox + m_HTildeDX[index].real() * lambda;
				m_Vertices[index1 + N + Nplus1 * N].z = m_Vertices[index1 + N + Nplus1 * N].oz + m_HTildeDZ[index].real() * lambda;

				m_Vertices[index1 + N + Nplus1 * N].nx = n.x;
				m_Vertices[index1 + N + Nplus1 * N].ny = n.y;
				m_Vertices[index1 + N + Nplus1 * N].nz = n.z;
			}
			if (n_prime == 0) {
				m_Vertices[index1 + N].y = m_HTilde[index].real();

				m_Vertices[index1 + N].x = m_Vertices[index1 + N].ox + m_HTildeDX[index].real() * lambda;
				m_Vertices[index1 + N].z = m_Vertices[index1 + N].oz + m_HTildeDZ[index].real() * lambda;

				m_Vertices[index1 + N].nx = n.x;
				m_Vertices[index1 + N].ny = n.y;
				m_Vertices[index1 + N].nz = n.z;
			}
			if (m_prime == 0) {
				m_Vertices[index1 + Nplus1 * N].y = m_HTilde[index].real();

				m_Vertices[index1 + Nplus1 * N].x = m_Vertices[index1 + Nplus1 * N].ox + m_HTildeDX[index].real() * lambda;
				m_Vertices[index1 + Nplus1 * N].z = m_Vertices[index1 + Nplus1 * N].oz + m_HTildeDZ[index].real() * lambda;

				m_Vertices[index1 + Nplus1 * N].nx = n.x;
				m_Vertices[index1 + Nplus1 * N].ny = n.y;
				m_Vertices[index1 + Nplus1 * N].nz = n.z;
			}
		}
	}

}


// ***** DEPRECATED *****
//float uniformRandomVariable() {
//	float randNum = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//	return randNum;
//}
//
//std::complex<float> gaussianRandomVariable() {
//	float x1, x2, w;
//	do {
//		x1 = 2.f * uniformRandomVariable() - 1.f;
//		x2 = 2.f * uniformRandomVariable() - 1.f;
//		w = x1 * x1 + x2 * x2;
//	} while (w >= 1.f);
//	w = sqrt((-2.f * log(w)) / w);
//	std::complex<float> complexNum(x1 * w, x2 * w);
//	return complexNum;
//
//}

//std::cout << "TEST VERTICES: \n";
	//for (int i = 0; i < vertices.size(); i++) {
	//	std::cout << vertices[i] << ", ";
	//	if (i % 3 == 0) std::cout << "\n";
	//}
	//std::cout << "TEST INDICES: \n";
	//for (int i = 0; i < indices.size(); i++) {
	//	std::cout << indices[i] << ", ";
	//	if (i % 3 == 0) std::cout << "\n";
	//}
	//std::cout << "VERTICES: \n";
	//for (int i = 0; i < m_Vertices.size(); i++) {
	//	std::cout << m_Vertices[i].x << ", " << m_Vertices[i].y << ", " << m_Vertices[i].z << ", \n";
	//}
	//std::cout << "INDICES: \n";
	//for (int i = 0; i < m_Indices.size(); i++) {
	//	printUVec3(m_Indices[i]);
	//}

