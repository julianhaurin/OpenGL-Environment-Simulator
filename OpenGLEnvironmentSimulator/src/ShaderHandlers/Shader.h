
// Notes:
// add common file-reading capability (right now they are split into separate functions within child classes)

#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

// Shader handler parent class - provides common OpenGL shader functions 
class Shader
{
public:
	virtual void InitializeShaders() = 0; // initialize shader objects
	void UseProgram() const;

	// uniform setting functions //
	void SetBool(const std::string& name, const bool value) const;
	void SetInt(const std::string& name, const int32_t value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

protected:
	unsigned int m_ID;

	bool checkCompilationSuccess(const uint32_t in_shaderID, const std::string in_type) const;

};

