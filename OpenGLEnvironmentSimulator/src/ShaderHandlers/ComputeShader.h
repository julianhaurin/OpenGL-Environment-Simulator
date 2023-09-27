
#pragma once

#include "Shader.h"

#include <sstream>
#include <fstream>

class ComputeShader : public Shader
{
public:
	ComputeShader(const char* in_compShaderPath);

private:
	std::string m_compShaderCode;

	// compiles compute shader and checks for errors
	void InitializeShaders() override;

	// reads source code from provided file and updates member variable
	void retrieveShaderSourceCode(const char* in_compShaderPath);

};

