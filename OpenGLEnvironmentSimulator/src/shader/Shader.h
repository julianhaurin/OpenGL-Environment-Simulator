
#pragma once

#include <iostream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

class Shader
{
public:
	virtual void CompileShaders() = 0;
	void UseProgram();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

protected:
	unsigned int m_ID;

	void checkCompilationErrors(uint32_t in_shaderID, std::string in_type) const;

};