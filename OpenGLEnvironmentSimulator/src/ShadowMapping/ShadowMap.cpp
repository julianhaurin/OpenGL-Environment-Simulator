
#include "ShadowMap.h"

ShadowMap::ShadowMap(const uint32_t in_shadowHeight, const uint32_t in_shadowWidth)
	: m_frameBufferID(0), m_depthMapID(0), 
	  m_shadowHeight(in_shadowHeight), m_shadowWidth(in_shadowWidth),
	  m_DepthShader(ShaderProgram("./shaders/depthMapVertShader.glsl", "./shaders/depthMapFragShader.glsl"))
{

	// initialize framebuffer //
	glGenFramebuffers(1, &m_frameBufferID);

	// initialize texture //
	glGenTextures(1, &m_depthMapID);
	glBindTexture(GL_TEXTURE_2D, m_depthMapID);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		m_shadowWidth, m_shadowHeight, 0, 
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL
	);

	// texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// frame buffer settings
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_frameBufferID, 0);

	// explicitly tell OpenGL to disregard color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

}

// Renders scene to depth map (before models are rendered)
void ShadowMap::PrepareRendering() {

	glViewport(0, 0, m_shadowWidth, m_shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

	glClear(GL_DEPTH_BUFFER_BIT);

	// Configure shader matrices
	glm::mat4 lightSpaceMatrix = getLightSpaceMatrix();

	// Set up shader
	m_DepthShader.UseProgram();
	m_DepthShader.SetMat4("u_LightSpaceMatrix", lightSpaceMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind frame buffer

}

glm::mat4 ShadowMap::getLightSpaceMatrix() {

	float nearPlane = 1.0f;
	float farPlane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;


}