
#pragma once

// Notes:
// do I need to add functions like:
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClearColor(0.05f, 0.2f, 0.3f, 1.0f);
// to the Update() function?
// update 9/6 - i think the destructor is being called??? like when its not supposed to
// which is why m_GridVBO wont render but VBO will
// could also use pocketfft or kissfft or fftw
// change order of member initalization list in .cpp file

// UPDATE RENDER() FUNCTION IN INTERFACE 

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

//#include "../Utility/PGFFT/PGFFT.h"
#include "../Utility/PocketFFT/PocketFFT.h"

#include "../Utility/FFT.h"
#include "../ShaderHandlers/ShaderProgram.h"
#include "../ShaderHandlers/ComputeShader.h"

#include "../VertexBuffer.h"
#include "../ElementBuffer.h"


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

	void Render(const float time, glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_LightPos, glm::vec3 in_CamPos) override;

private:
	// shader program
	ShaderProgram m_OceanShaderProgram;
	ComputeShader m_OceanComputeShader;

	//VertexBuffer m_VBO;
	//ElementBuffer m_EBO;

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

	// FFT data
	FFT m_FFT;
	std::vector<std::complex<float>> m_HTilde;
	std::vector<std::complex<float>> m_HTildeSlopeX;
	std::vector<std::complex<float>> m_HTildeSlopeZ;
	std::vector<std::complex<float>> m_HTildeDX;
	std::vector<std::complex<float>> m_HTildeDZ;

	// PocketFFT
	const pocketfft::shape_t m_PFFT_shape;
	const pocketfft::stride_t m_PFFT_stride;

	void Initialize();
	void DeallocateResources();

	// Tessendforf equations
	float phillipsSpectrum(const int32_t mPrime, const int32_t nPrime) const;
	float dispersionRelation(int32_t mPrime, int32_t nPrime) const;
	std::complex<float> hTilde_0(int32_t nPrime, int32_t mPrime) const;
	std::complex<float> hTilde(const float t, const int32_t nPrime, const int32_t mPrime) const;

	heightDisplacementNormal evalWaveData(glm::vec2 x, float time);
	// evaluates new wave height
	void EvaluateWavesDFT(const float time); // discrete fourier transform
	void EvaluateWavesFFT(const float time); // fast fourier transform

};