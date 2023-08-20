
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

class ShaderProgram : public Shader
{
public: 
	ShaderProgram(const char* in_vertexShaderPath, const char* in_fragmentShaderPath);

	// compiles and links shaders into program object
	void CompileShaders();
 

private:

	std::string m_vertShaderCode;
	std::string m_fragShaderCode;

	void checkLinkingErrors(uint32_t in_programID) const;

};
