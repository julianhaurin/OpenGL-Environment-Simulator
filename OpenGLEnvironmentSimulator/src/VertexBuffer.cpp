
// VBO Class - 9/10/23

#include "VertexBuffer.h"

// Public Methods //

VertexBuffer::VertexBuffer()
	: m_isInitialized(false)
{
	glGenBuffers(1, &m_ID);
}

VertexBuffer::VertexBuffer(const std::vector<GLfloat> in_VertexData)
	: VertexBuffer() // calls default constructor
{

	Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(in_VertexData[0]) * in_VertexData.size(), &in_VertexData[0], GL_STATIC_DRAW);
	m_isInitialized = true;

}

VertexBuffer::~VertexBuffer() {

	glDeleteVertexArrays(1, &m_ID);

}

bool VertexBuffer::Initialize(const std::vector<GLfloat> in_VertexData) {

	if (m_isInitialized) return false; // checks to see if VBO data has been initialized (see notes)

	Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(in_VertexData[0]) * in_VertexData.size(), &in_VertexData[0], GL_STATIC_DRAW);

	m_isInitialized = true;
	return true;
}

// Private Methods //

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);

}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

