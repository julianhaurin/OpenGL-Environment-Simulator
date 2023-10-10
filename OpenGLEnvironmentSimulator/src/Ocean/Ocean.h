
// Notes:
// could use pocketfft or kissfft or fftw
// more in ocean.cpp

#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>
#include <complex>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ShaderHandlers/ShaderProgram.h"
#include "../ShaderHandlers/ComputeShader.h"

#include "../Objects/Light.h"

// data within ocean grid vertex
struct OceanVertex {
	GLfloat x, y, z; // current position
	GLfloat padding1;
	GLfloat ox, oy, oz; // original position (for offset calc)
	GLfloat padding2;
	GLfloat nx, ny, nz; // normal
	GLfloat padding3;
	GLfloat a, b, c; // htilde
	GLfloat padding4;
	GLfloat _a, _b, _c; // discrete fourier transform //
	GLfloat padding5;

};

// return data of evalWaveData() func
struct heightDisplacementNormal {
	std::complex<float> height;
	glm::vec2 displacement;
	glm::vec3 normal;

};

class Ocean
{
public:
	const static float gravityConst; // 9.81f

	Ocean(
		const uint32_t gridDimensions, 
		const float waveHeight_A = 1.0f, 
		glm::vec2 windDir_w = glm::vec2(1.0f, 1.0f), 
		const float length = 8, 
		const LightData in_lightData = Light::getDefaultLightData()
	);
	~Ocean();

	void Bind();
	void Render(const float time, glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_CamPos);

private:

	// shader program
	ShaderProgram m_OceanShaderProgram;
	ComputeShader m_OceanComputeShader;

	GLuint m_GridVBO;
	GLuint m_GridVAO;
	GLuint m_GridEBO;

	GLuint m_GridSSBO;

	// shader attribute locations
	GLint m_PositionAttrib;
	GLint m_NormalAttrib;
	//GLint m_TextureAttrib;

	// grid vertices and indices (VBO and EBO data)
	std::vector<OceanVertex> m_Vertices;
	std::vector<GLuint> m_Indices;

	// ocean parameters
	const int m_GridSideDimension;
	const float m_phillipsConstant_A;
	const glm::vec2 m_windDir_w;
	const float m_Length;

	const LightData m_LightData;

	void Initialize();
	void DeallocateResources();

	void configureShaderProgram(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_CamPos);

	// Tessendforf equations
	float phillipsSpectrum(const int32_t mPrime, const int32_t nPrime) const;
	float dispersionRelation(int32_t mPrime, int32_t nPrime) const;
	std::complex<float> hTilde_0(int32_t nPrime, int32_t mPrime) const;
	std::complex<float> hTilde(const float t, const int32_t nPrime, const int32_t mPrime) const;

	heightDisplacementNormal evalWaveData(glm::vec2 x, float time);
	// evaluates new wave height
	void EvaluateWavesDFT(const float time); // discrete fourier transform
	//void EvaluateWavesFFT(const float time); // fast fourier transform

};

// update 9/6 - i think the destructor is being called??? like when its not supposed to