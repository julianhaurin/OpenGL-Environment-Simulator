
#include "OceanTexture.h"

OceanTexture::OceanTexture(const uint32_t in_height, const uint32_t in_width) 
	: m_texturedID(0), m_imageHeight(in_height), m_imageWidth(in_width),
	  m_ComputeShader(ComputeShader("./shaders/oceanCompShader.glsl"))
{

	glGenTextures(1, &m_texturedID);

	// stackoverflow.com/questions/8866904/differences-and-relationship-between-glactivetexture-and-glbindtexture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texturedID);

	// texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_imageWidth, m_imageHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindImageTexture(0, m_texturedID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

}

void OceanTexture::Render() {

	m_ComputeShader.UseProgram();
	glDispatchCompute(m_imageWidth, m_imageHeight, 1); // executes compute shader

	// ensures compute shader finishes execution before other stages of render pipeline
	glMemoryBarrier(GL_ALL_BARRIER_BITS); 

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texturedID);

}