
// *** IN-PROGRESS *** //

#pragma once

#include <cstdint>
#include "GL/glew.h"

#include "../ShaderHandlers/ComputeShader.h"

class OceanTexture 
{

public:
	OceanTexture(const uint32_t in_height, const uint32_t in_width);

	void Render();

private:
	uint32_t m_texturedID;

	const uint32_t m_imageHeight;
	const uint32_t m_imageWidth;

	ComputeShader m_ComputeShader;

};
