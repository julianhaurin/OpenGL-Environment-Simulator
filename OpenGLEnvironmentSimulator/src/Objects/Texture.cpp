
#include "Texture.h"

// Public Methods //

Texture::Texture(const std::string in_texturePath)
	: m_texturePath(in_texturePath), m_textureID(0)
{

	loadAndGenTexture();

}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::setTexParams() {

	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

// Private Methods //

bool Texture::loadAndGenTexture() {

	glGenTextures(1, &m_textureID);
	Bind();

	setTexParams();

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(m_texturePath.c_str(), &m_width, &m_height, &m_nrChannels, 0);

	if (data == nullptr) {
		std::cout << "[J] ERROR - Unable to load texture data from path: " << m_texturePath << std::endl;
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	std::cout << "[J] Successfully loaded texture data from: " << m_texturePath << "\n";
	return true;

}