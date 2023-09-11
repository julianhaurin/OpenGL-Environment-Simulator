
// VBO Class - 9/10/23

#include "VertexBuffer.h"

// Public Methods //

VertexBuffer::VertexBuffer(const std::vector<GLfloat> in_VertexData) {

	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(in_VertexData[0]) * in_VertexData.size(), &in_VertexData[0], GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer() {

	glDeleteVertexArrays(1, &m_ID);

}

// Private Methods //

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);

}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

