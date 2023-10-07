
// Shadow mapping functionality

#pragma once

#include <cstdint>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ShaderHandlers/ShaderProgram.h"

class ShadowMap
{

public:
	ShadowMap(const uint32_t in_shadowHeight, const uint32_t in_shadowWidth);

	void PrepareRendering();
	glm::mat4 getLightSpaceMatrix();

private:
	
	GLuint m_frameBufferID; // depth map frame buffer object (FBO)
	GLuint m_depthMapID; // depth map texture ID

	// depth map resolution
	const uint32_t m_shadowHeight;
	const uint32_t m_shadowWidth;

	// depth map shader
	ShaderProgram m_DepthShader;

};
