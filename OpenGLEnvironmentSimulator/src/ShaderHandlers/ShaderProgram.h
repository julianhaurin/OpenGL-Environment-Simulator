
#pragma once

#include "Shader.h"

#include <iostream>
#include <string>
#include <cstdint>
#include <sstream>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

// represents a shader program OpenGL object
class ShaderProgram : public Shader
{
public: 
	ShaderProgram(const char* in_vertexShaderPath, const char* in_fragmentShaderPath);
	GLint getID();

private:

	// shader source code //
	std::string m_vertShaderCode;
	std::string m_fragShaderCode;

	void InitializeShaders(); // compiles and links shaders into program object

	// reads source code from provided files and updates member variables
	void retrieveShaderSourceCode(const char* in_vertexShaderPath, const char* in_fragmentShaderPath);
	// checks for shader linking errors
	bool checkLinkingSuccess(const int32_t in_programID) const;

};

