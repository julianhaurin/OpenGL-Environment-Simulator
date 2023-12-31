
//*** work in progress **//

#include "ComputeShader.h"

ComputeShader::ComputeShader(const char* in_compShaderPath) 
	: m_compShaderCode("")
{
	retrieveShaderSourceCode(in_compShaderPath); // get source code from provided file
	InitializeShaders(); // setup and initialize compute shader

}

// compiles compute shader and checks for errors
void ComputeShader::InitializeShaders() {
	const char* compShaderCode = m_compShaderCode.c_str();

	GLuint computeID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeID, 1, &compShaderCode, NULL);
	glCompileShader(computeID);

	if (checkCompilationSuccess(computeID, "compute") == true) {
		std::cout << "[J] Shader program (ID: " << computeID << ") successfully compiled compute shader! \n";
	}
	
	m_ID = glCreateProgram();
	glAttachShader(m_ID, computeID);
	glLinkProgram(m_ID);

	std::cout << std::endl;
	
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
		std::cout << "[J] - ERROR: Compute shader failed to successfully read provided file: " << 
			err.what() << std::endl;
	}

}

