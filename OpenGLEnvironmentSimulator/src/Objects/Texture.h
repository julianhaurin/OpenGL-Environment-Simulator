
// Represents an image texture and associated OpenGL objects

// Notes:
// abstract setTexParams() function and allow for custom settings

#pragma once

#include "stb_image.h"

#include <iostream>
#include <string>

#include "GL/glew.h"

class Texture
{
public:
	Texture(const std::string in_texturePath);
	void Bind(); // binds texture objects

private:
	const std::string m_texturePath; // file path to texture image

	GLuint m_textureID;

	int m_width; 
	int m_height;
	int m_nrChannels;

	bool loadAndGenTexture(); // loads texture data and binds to m_textureID
	void setTexParams(); // add params to make chaging settings easier
	

};
