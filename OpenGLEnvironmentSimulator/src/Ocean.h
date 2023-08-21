
#pragma once

// Notes:
// do I need to add functions like:
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClearColor(0.05f, 0.2f, 0.3f, 1.0f);
// to the Update() function?
//

#include <iostream>
#include <vector>
#include <cstdint>

#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

//struct OceanVertex {
//	glm::vec3 position;
//	OceanVertex() : position(glm::vec3(0.0f, 0.0f, 0.0f)) {};
//	OceanVertex(glm::vec3 in_pos) : position(in_pos) {};
//};

class Ocean
{
public:
	Ocean(const uint32_t gridLength = 256);
	~Ocean();

	void Initialize();
	void Update();

private:
	const int m_GridSideLength;

	uint32_t m_GridVBO;
	uint32_t m_GridVAO;
	uint32_t m_GridEBO;

	void DeallocateResources();

	float phillipsSpectrum(int gridLenRange) const;

};