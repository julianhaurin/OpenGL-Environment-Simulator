
// Notes:
// phillips spectrum and dispersion functions both might not be accurate,
// other implementations have a 'length' parameter and use
// m - N / length for k vector
//
// wind speed is controlled by w vector (wind direction vector) 

#include "Ocean.h"

const float Ocean::gravityConst = 9.81f;

Ocean::Ocean(const uint32_t gridLength, const float waveHeight_A, glm::vec2 windDir_w)
	: m_GridSideLength(gridLength), m_phillipsConstant_A(waveHeight_A), m_windDir_w(windDir_w)
{
	assert(m_GridSideLength && !(m_GridSideLength & (m_GridSideLength - 1))); // grid length == power of 2

}

Ocean::~Ocean() 
{
	DeallocateResources();
}

void Ocean::Initialize() {

	// generate grid
	const uint32_t verticesCount = (m_GridSideLength + 1) * (m_GridSideLength + 1);
	const uint32_t indicesCount = m_GridSideLength * m_GridSideLength * 2;

	std::vector<glm::vec3> vertices(verticesCount);
	std::vector<glm::uvec3> indices(indicesCount);

	// populate vertices
	uint32_t idx = 0;
	for (float i = -(m_GridSideLength / 2); i <= (m_GridSideLength / 2); ++i) {
		for (float j = -(m_GridSideLength / 2); j <= (m_GridSideLength / 2); ++j) {
			vertices[idx] = glm::vec3((float)j, 0.0f, (float)i);
			idx++;

		}
	}

	// populate indices with clockwise winding
	idx = 0;
	for (int i = 0; i < m_GridSideLength; ++i) {
		int row1 = i * (m_GridSideLength + 1);
		int row2 = (i + 1) * (m_GridSideLength + 1);

		for (int j = 0; j < m_GridSideLength; ++j) {
			indices[idx++] = glm::uvec3(row1 + j, row1 + j + 1, row2 + j + 1);
			indices[idx++] = glm::uvec3(row1 + j, row2 + j + 1, row2 + j);

		}
	}

	assert(verticesCount == vertices.size());
	assert(indicesCount == indices.size());

	// bind grid VBO, VAO, and EBO
	glGenBuffers(1, &m_GridVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_GridVAO);
	glBindVertexArray(m_GridVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &m_GridEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GridEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);


}

void Ocean::Update() {

	glBindVertexArray(m_GridVAO);
	glDrawElements(GL_TRIANGLES, (m_GridSideLength + 1) * (m_GridSideLength + 1) * 2 * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Ocean::DeallocateResources() {

	glDeleteVertexArrays(1, &m_GridVBO);
	glDeleteBuffers(1, &m_GridVAO);
	glDeleteBuffers(1, &m_GridEBO);

}

float Ocean::dispersionRelation(int gridLenRange) const {

	float w_0 = 2.0f * M_PI / 50.0f;
	float kx = M_PI * (2 * gridLenRange - m_GridSideLength);
	float kz = M_PI * (2 * gridLenRange - m_GridSideLength);
	return floor(sqrt(gravityConst * sqrt(kx * kx + kz * kz)) / w_0) * w_0;

}

float Ocean::phillipsSpectrum(int gridLenRange) const {

	const float kx = (2 * M_PI * gridLenRange - m_GridSideLength);
	const float ky = (2 * M_PI * gridLenRange - m_GridSideLength);

	glm::vec2 k(kx, ky);

	const float kLen2 = glm::length(k) * glm::length(k);
	const float kLen4 = kLen2 * kLen2;

	const float kDotw = glm::dot(glm::normalize(k), glm::normalize(m_windDir_w));
	const float magnitudeKW = glm::length(kDotw);

	const float windSpeed_V = glm::length(m_windDir_w);

	const float L = (windSpeed_V * windSpeed_V) / gravityConst;
	const float kL2 = (L * L) * kLen2;

	float damping = 0.001;
	float l2 = L * L * damping * damping;

	return m_phillipsConstant_A * (std::exp(-1.0f / (kL2)) / kLen4) 
		 * magnitudeKW * magnitudeKW * std::exp(-kLen2 * l2);

}

