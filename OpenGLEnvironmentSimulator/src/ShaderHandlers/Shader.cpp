
#include "Shader.h"

// makes this shader program current
void Shader::UseProgram() const {
	glUseProgram(m_ID);
}

// uniform setting functions //
void Shader::SetBool(const std::string& name, const bool value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, const int32_t value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const float value) const {
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& vec) const {
	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vec) const {
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vec[0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// checks for the compilation success of the provided shaderID
bool Shader::checkCompilationSuccess(const uint32_t in_shaderID, const std::string in_type) const {

	GLint success;
	char infoLog[1024];

	glGetShaderiv(in_shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(in_shaderID, 1024, NULL, infoLog);
		std::cout << "[J] - ERROR: Shader program (ID: " << m_ID << ") failed to compile " 
			<< in_type << " shader successfully: " << infoLog << std::endl;
	}

	return success;

}

