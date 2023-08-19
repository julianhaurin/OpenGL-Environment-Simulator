
#include "Shader.h"

Shader::Shader(const char* in_vertexShaderPath, const char* in_fragmentShaderPath) {

	// creates file streams and reads shader source code into strings
	std::ifstream vertFileStream;
	std::ifstream fragFileStream;

	vertFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertFileStream.open(in_vertexShaderPath);
		fragFileStream.open(in_fragmentShaderPath);

		std::stringstream vertStrStream;
		std::stringstream fragStrStream;

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

void Shader::CompileShaders() {

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

void Shader::UseProgram() {
	glUseProgram(m_ID);
}

void Shader::SetBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompilationErrors(uint32_t in_shaderID, std::string in_type) const {

	GLint success;
	char infoLog[1024];

	glGetShaderiv(in_shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(in_shaderID, 1024, NULL, infoLog);
		std::cout << "[J] - ERROR: Failed to compile " << in_type << " shader successfully: " << infoLog << std::endl;
	}

}

void Shader::checkLinkingErrors(uint32_t in_programID) const {

	GLint success;
	char infoLog[1024];

	glGetProgramiv(in_programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(in_programID, 1024, NULL, infoLog);
		std::cout << "[J] - ERROR: Failed to successfully link shader program: " << infoLog << std::endl;
	}
}

