
#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include <sstream>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

class Shader
{
public: 
	unsigned int m_ID;

	Shader(const char* in_vertexShaderPath, const char* in_fragmentShaderPath);

	// compiles and links shaders into program object
	void CompileShaders();

	void UseProgram();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:

	std::string m_vertShaderCode;
	std::string m_fragShaderCode;

	void checkCompilationErrors(uint32_t in_shaderID, std::string in_type) const;
	void checkLinkingErrors(uint32_t in_programID) const;

};
