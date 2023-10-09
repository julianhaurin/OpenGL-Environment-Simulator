
// Class abstraction for shader program that accepts vertex and fragment shaders

#pragma once

#include "Shader.h" // parent class

#include <iostream>
#include <string>
#include <cstdint>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../Utility/utility.h"

// represents a shader program OpenGL object
class ShaderProgram : public Shader
{
public: 
	// accepts paths to vertex and fragment shaders
	ShaderProgram(const char* in_vertexShaderPath, const char* in_fragmentShaderPath);

private:

	// shader source code //
	std::string m_vertShaderCode;
	std::string m_fragShaderCode;

	void InitializeShaders() override; // compiles and links shaders into program object

	// reads source code from provided files and updates member variables
	void retrieveShaderSourceCode(const char* in_vertexShaderPath, const char* in_fragmentShaderPath);
	// checks for shader linking errors
	bool checkLinkingSuccess(const int32_t in_programID) const;

};

