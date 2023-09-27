
// Class Abstraction for OpenGL Vertex Buffer Object (VBO) - 9/10/23

// Notes:
// is GLfloat / GLuint etc. best practice? 
// combine with EBO class? 
// add functionality to control draw type (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc)
// 
// want to be able to initialize object before having access to vector data
// solution for now (9/12/23): have two constructs and an init func that adds data 
// is updated VBO data a thing (and decent practice) in OpenGL - initializing multiple times?
// change isInit var name, same in element buffer class
//
// Status 9/12/23 - calling glGenBuffers(1, m_ID) in constructor absolutely clusterfucks 
// the whole program and nothing renders.. tf

#pragma once

#include <vector>

#include "GL/glew.h"

class VertexBuffer
{

public:
	VertexBuffer();
	VertexBuffer(const std::vector<GLfloat> in_VertexData);
	~VertexBuffer();

	bool Initialize(const std::vector<GLfloat> in_VertexData); // returns true on success

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_ID;
	bool m_isInitialized;

};