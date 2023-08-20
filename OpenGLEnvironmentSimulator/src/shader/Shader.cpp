
#include "Shader.h"

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