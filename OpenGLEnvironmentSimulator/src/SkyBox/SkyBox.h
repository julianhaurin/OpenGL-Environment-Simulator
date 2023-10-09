
// Combine with Texture class?

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <GL/glew.h>

#include "../ShaderHandlers/ShaderProgram.h"
#include "../Objects/Texture.h"

class SkyBox
{

public:

	static const std::vector<std::string> s_imageFaces;
	static const std::vector<float> s_skyBoxVertices;

	SkyBox(const std::string in_textureFolderPath);
	
	void Bind();
	void Render(glm::mat4 in_viewMat, const glm::mat4 in_projMat);
	

private:
	const std::string m_textureFolderPath;

	ShaderProgram m_ShaderProgram;

	GLuint m_textureID;

	GLuint m_VBO;
	GLuint m_VAO;

	int m_width;
	int m_height;
	int m_nrChannels;

	bool loadAndGenSkyBoxTex();
	void setupSkybox();

};
