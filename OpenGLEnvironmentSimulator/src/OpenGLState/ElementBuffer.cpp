
// EBO Class - 9/10/23

#include "ElementBuffer.h"

// Public Methods //

ElementBuffer::ElementBuffer()
	: m_isInitialized(false)
{
	glGenBuffers(1, &m_ID);
}

ElementBuffer::ElementBuffer(std::vector<GLuint> in_IndexData) 
	: ElementBuffer() // calls default constructor
{
	
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(in_IndexData[0]) * in_IndexData.size(), &in_IndexData[0], GL_STATIC_DRAW);
	m_isInitialized = true;

}


ElementBuffer::~ElementBuffer() {

	glDeleteVertexArrays(1, &m_ID);

}

bool ElementBuffer::Initialize(std::vector<GLuint> in_IndexData) {

	if (m_isInitialized == true) return false;

	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(in_IndexData[0]) * in_IndexData.size(), &in_IndexData[0], GL_STATIC_DRAW);
	m_isInitialized = true;

}

void ElementBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void ElementBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

