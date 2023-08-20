
#pragma once

#include "Shader.h"

#include <sstream>
#include <fstream>

class ComputeShader : public Shader
{
public:
	ComputeShader(const char* in_compShaderPath) {

		std::ifstream compFileStream;
		compFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			std::stringstream compStrStream;
			
			compFileStream.open(in_compShaderPath);
			compStrStream << compFileStream.rdbuf();
			compFileStream.close();

			m_compShaderCode = compStrStream.str();

		}
		catch (std::ifstream::failure& err) {
			std::cout << "[J] - ERROR: Computer shader failed to successfully read provided file: " << err.what() << std::endl;
		}

	}

	void CompileShaders() {
		const char* compShaderCode = m_compShaderCode.c_str();

		m_ID = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(m_ID, 1, &compShaderCode, NULL);
		glCompileShader(m_ID);

		checkCompilationErrors(m_ID, "compute");

	}

private:
	std::string m_compShaderCode;

};