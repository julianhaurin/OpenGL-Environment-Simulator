
// Notes:
// phillips spectrum and dispersion functions both might not be accurate,
// other implementations have a 'length' parameter and use
// m - N / length for k vector, i think has to do with different for loop variables (i vs n_prime)
//
// wind speed is controlled by w vector (wind direction vector) 
// add length variable to adjust in-world size of ocean grid

// mPrime and nPrime are now values within range [0, M] and [0, N] respectively, 
// and are converted to proper ranges within functions when necessary


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
	  m_OceanShaderProgram(ShaderProgram("./shaders/oceanVertShader.glsl", "./shaders/oceanFragShader.glsl"))
{
	assert(m_GridSideDimension && !(m_GridSideDimension & (m_GridSideDimension - 1))); // m_GridSideDimension == power of 2

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

			m_Vertices[idx].ox = static_cast<float>(n - m_GridSideDimension / 2.0f); // *m_Length / m_GridSideDimension;
			m_Vertices[idx].oy = 0.0f;
			m_Vertices[idx].oz = static_cast<float>(m - m_GridSideDimension / 2.0f); // *m_Length / m_GridSideDimension;

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

	// generate and bind grid VBO, VAO, and EBO
	glGenBuffers(1, &m_GridVBO);
	glGenBuffers(1, &m_GridEBO);
	glGenVertexArrays(1, &m_GridVAO);

	glBindVertexArray(m_GridVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices[0]) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GridEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_PositionAttrib);
	glVertexAttribPointer(m_PositionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	std::cout << "[J] - Ocean object successfully initialized! \n\n";
	
	
	std::vector<float> vertices;
	for (int i = 0; i < m_Vertices.size(); i++) {
		vertices.push_back(m_Vertices[i].x);
		vertices.push_back(m_Vertices[i].y);
		vertices.push_back(m_Vertices[i].z);
	}
	std::vector<uint32_t> indices;
	for (int i = 0; i < m_Indices.size(); i++) {
		indices.push_back(m_Indices[i]);
	}
	
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
	
}

void Ocean::Render(const float time, glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat) { // render function

	//evaluateWavesDFT(time);

	m_OceanShaderProgram.UseProgram();
	m_OceanShaderProgram.SetMat4("u_Model", in_ModelMat);
	m_OceanShaderProgram.SetMat4("u_View", in_ViewMat);
	m_OceanShaderProgram.SetMat4("u_Projection", in_ProjeMat);

	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBindVertexArray(m_GridVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GridEBO);

	//glDrawArrays(GL_TRIANGLES, 0, 1 * 3);
	glDrawElements(GL_TRIANGLES, m_Indices.size() * 3, GL_UNSIGNED_INT, 0);

}

void Ocean::DeallocateResources() {

	glDeleteVertexArrays(1, &m_GridVBO);
	glDeleteBuffers(1, &m_GridVAO);
	glDeleteBuffers(1, &m_GridEBO);

}

// Tessendorf equations //

// Tessendorf equation 35
float Ocean::dispersionRelation(int32_t nPrime, int32_t mPrime) const {
	float w_0 = 2.0f * M_PI / 200.0f; // 200.0f = T (time after which cycle will repeat, Tessendorf equation 34)
	float kx = M_PI * (2 * nPrime - m_GridSideDimension); // / m_Length;
	float kz = M_PI * (2 * mPrime - m_GridSideDimension); // / m_Length;
	return floor(sqrt(gravityConst * sqrt(kx * kx + kz * kz)) / w_0) * w_0;

}

// Tessendorf equation 40/41
float Ocean::phillipsSpectrum(const int32_t mPrime, const int32_t nPrime) const {

	const float kx = M_PI * (2 * nPrime - m_GridSideDimension); // / m_Length;
	const float ky = M_PI * (2 * mPrime - m_GridSideDimension); // / m_Length;
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

	return randNum * sqrt(phillipsSpectrum(nPrime, mPrime) / 2.0f);

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
		kz = 2.0f * M_PI * (m - m_GridSideDimension / 2.0f); // / m_Length;

		for (float n = 0; n <= m_GridSideDimension; ++n) {
			kx = 2.0f * M_PI * (n - m_GridSideDimension / 2.0f); // / m_Length;
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

void Ocean::evaluateWavesDFT(float time) {

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



