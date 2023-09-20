
// Notes:
// add common file-reading capability (right now they are split into separate functions within child classes)
// checkCompilationSuccess return type is weird ... check, lots of changes 

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
	void UseProgram() const;

	// uniform setting functions //
	void SetBool(const std::string& name, const bool value) const;
	void SetInt(const std::string& name, const int32_t value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetVec3(const std::string& name, const glm::vec3& vec) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;

protected:
	GLuint m_ID;

	virtual void InitializeShaders() = 0; // initialize shader objects
	bool checkCompilationSuccess(const uint32_t in_shaderID, const std::string in_type) const;

};

