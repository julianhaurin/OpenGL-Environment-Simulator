
// Class Abstraction for OpenGL Vertex Buffer Object (VBO) - 9/10/23

// Notes:
// is GLfloat / GLuint etc. best practice? 
// combine with EBO class? 
// add functionality to control draw type (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc)

#pragma once

#include <vector>

#include "GL/glew.h"

class VertexBuffer
{

public:
	VertexBuffer(const std::vector<GLfloat> in_VertexData);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;

};