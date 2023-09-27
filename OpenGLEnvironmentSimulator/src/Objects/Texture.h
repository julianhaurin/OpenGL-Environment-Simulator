
#pragma once

#include "stb_image.h"

#include <iostream>
#include <string>

#include "GL/glew.h"

class Texture
{

public:
	Texture(const std::string in_texturePath);
	void Bind();
	void setTexParams(); // add params to make chaging settings easier

private:
	const std::string m_texturePath;

	GLuint m_textureID;

	int m_width; 
	int m_height;
	int m_nrChannels;

	bool loadAndGenTexture();
	

};
