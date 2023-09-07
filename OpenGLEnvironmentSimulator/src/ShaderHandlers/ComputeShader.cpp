
#include "ComputeShader.h"

ComputeShader::ComputeShader(const char* in_compShaderPath) 
	: m_compShaderCode("")
{
	retrieveShaderSourceCode(in_compShaderPath);
}

// compiles compute shader and checks for errors
void ComputeShader::InitializeShaders() {
	const char* compShaderCode = m_compShaderCode.c_str();

	m_ID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(m_ID, 1, &compShaderCode, NULL);
	glCompileShader(m_ID);

	if (checkCompilationSuccess(m_ID, "compute") == true) {
		std::cout << "[J] Shader program (ID: " << m_ID << ") successfully compiled compute shader! \n";
	}

}

// reads source code from provided file and updates member variable
void ComputeShader::retrieveShaderSourceCode(const char* in_compShaderPath) {

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
		std::cout << "[J] - ERROR: Compute shader (ID: " << m_ID 
			<< ") failed to successfully read provided file: " << err.what() << std::endl;
	}

}

