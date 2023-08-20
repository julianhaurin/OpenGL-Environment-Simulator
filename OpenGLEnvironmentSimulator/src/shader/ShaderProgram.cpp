
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* in_vertexShaderPath, const char* in_fragmentShaderPath) {

	// creates file streams and reads shader source code into strings
	std::ifstream vertFileStream;
	std::ifstream fragFileStream;

	vertFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::stringstream vertStrStream;
		std::stringstream fragStrStream;

		vertFileStream.open(in_vertexShaderPath);
		fragFileStream.open(in_fragmentShaderPath);

		vertStrStream << vertFileStream.rdbuf();
		fragStrStream << fragFileStream.rdbuf();

		vertFileStream.close();
		fragFileStream.close();

		m_vertShaderCode = vertStrStream.str();
		m_fragShaderCode = fragStrStream.str();


	}
	catch (std::ifstream::failure& err) {
		std::cout << "[J] - ERROR: Shader program failed to successfully read shader files: " << err.what() << std::endl;

	}

}

void ShaderProgram::CompileShaders() {

	const char* vertShaderCode = m_vertShaderCode.c_str();
	const char* fragShaderCode = m_fragShaderCode.c_str();

	uint32_t vertShaderID;
	uint32_t fragShaderID;

	vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vertShaderCode, NULL);
	glCompileShader(vertShaderID);
	checkCompilationErrors(vertShaderID, "vertex");

	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fragShaderCode, NULL);
	glCompileShader(fragShaderID);
	checkCompilationErrors(fragShaderID, "fragment");

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertShaderID);
	glAttachShader(m_ID, fragShaderID);
	glLinkProgram(m_ID);
	checkLinkingErrors(m_ID);

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

}

void ShaderProgram::checkLinkingErrors(uint32_t in_programID) const {

	GLint success;
	char infoLog[1024];

	glGetProgramiv(in_programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(in_programID, 1024, NULL, infoLog);
		std::cout << "[J] - ERROR: Failed to successfully link shader program: " << infoLog << std::endl;
	}
}

