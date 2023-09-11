
// Class Abstraction for OpenGL Element Buffer Object (EBO) - 9/10/23

// Notes:
// notes from VertexBuffer.h
// 

#pragma once

#include <vector>

#include "GL/glew.h"

class ElementBuffer
{
public:

	ElementBuffer(std::vector<GLuint> in_IndexData);
	~ElementBuffer();

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;

};
