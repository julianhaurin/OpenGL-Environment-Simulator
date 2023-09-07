
#pragma once

#include "Shader.h"

#include <sstream>
#include <fstream>

class ComputeShader : public Shader
{
public:
	ComputeShader(const char* in_compShaderPath);

	// compiles compute shader and checks for errors
	void InitializeShaders();

private:
	std::string m_compShaderCode;

	// reads source code from provided file and updates member variable
	void retrieveShaderSourceCode(const char* in_compShaderPath);

};

