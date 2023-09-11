
// EBO Class - 9/10/23

#include "ElementBuffer.h"

// Public Methods //

ElementBuffer::ElementBuffer(std::vector<GLuint> in_IndexData) {

	glGenBuffers(1, &m_ID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(in_IndexData[0]) * in_IndexData.size(), &in_IndexData[0], GL_STATIC_DRAW);

}

ElementBuffer::~ElementBuffer() {

	glDeleteVertexArrays(1, &m_ID);

}

void ElementBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void ElementBuffer::Unbind() const {

}

