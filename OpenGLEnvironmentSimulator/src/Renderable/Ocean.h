
#pragma once

// Notes:
// do I need to add functions like:
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClearColor(0.05f, 0.2f, 0.3f, 1.0f);
// to the Update() function?
//

#include "Renderable.h"

#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include <random>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ShaderHandlers/ShaderProgram.h"

// data within ocean grid vertex
struct OceanVertex {
	GLfloat x, y, z; // current position
	GLfloat ox, oy, oz; // original position (for offset calc)
	GLfloat nx, ny, nz; // normal
	GLfloat a, b, c; // htilde
	GLfloat _a, _b, _c; // discrete fourier transform //

};

// return data of evalWaveData() func
struct heightDisplacementNormal {
	std::complex<float> height;
	glm::vec2 displacement;
	glm::vec3 normal;

};

class Ocean : public Renderable
{
public:
	const static float gravityConst; // 9.81f

	Ocean(const uint32_t gridDimensions = 8, const float waveHeight_A = 1.0f, glm::vec2 windDir_w = glm::vec2(1.0f, 1.0f), const float length = 8);
	~Ocean();

	void Render(const float time, glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat) override;

private:
	// shader program
	ShaderProgram m_OceanShaderProgram;

	// shader attribute locations
	GLint m_PositionAttrib;
	//GLint m_NormalAttrib;
	//GLint m_TextureAttrib;

	// grid vertices and indices (VBO and EBO data)
	std::vector<OceanVertex> m_Vertices;
	std::vector<GLuint> m_Indices;

	// ocean parameters
	const int m_GridSideDimension;
	const float m_phillipsConstant_A;
	const glm::vec2 m_windDir_w;
	const float m_Length;

	GLuint m_GridVBO;
	GLuint m_GridVAO;
	GLuint m_GridEBO;

	void Initialize();
	void DeallocateResources();

	// Tessendforf equations
	float phillipsSpectrum(const int32_t mPrime, const int32_t nPrime) const;
	float dispersionRelation(int32_t mPrime, int32_t nPrime) const;
	std::complex<float> hTilde_0(int32_t nPrime, int32_t mPrime) const;
	std::complex<float> hTilde(const float t, const int32_t nPrime, const int32_t mPrime) const;

	heightDisplacementNormal evalWaveData(glm::vec2 x, float time);
	void evaluateWavesDFT(float time); // evaluates new wave height

	// test stuff
	GLuint VBO, EBO, VAO;
	

};