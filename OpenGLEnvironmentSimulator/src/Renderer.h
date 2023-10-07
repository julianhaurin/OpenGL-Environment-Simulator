
#pragma once

// Notes:
// lots of optimizing and updating to do
// research batch rendering techniques and
// industry standard for rendering list of objects (model class)
//
// glfw callback function workaround 
// stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
// 
// init vbos n shit to 0

#include <iostream>
#include <cstdint>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera/Camera.h"

#include "Ocean/Ocean.h"
#include "ShadowMapping/ShadowMap.h"

#include "Objects/SkyBox.h"
#include "Objects/Model.h"
#include "Objects/Material.h"
#include "Objects/Light.h"

#include "Utility/utility.h"

// represents an OpenGL context and scene
class Renderer 
{
public:
	// initial window dimensions //
	static const uint32_t ScreenHeight;
	static const uint32_t ScreenWidth;

	Renderer();
	~Renderer();

	// render loop
	void addModel(std::shared_ptr<Model> in_model_p);
	void Run();

private:
    GLFWwindow* m_Window;
	Camera m_Camera;

	std::shared_ptr<ShadowMap> m_ShadowMap; // Shadow map object
	std::shared_ptr<Ocean> m_Ocean; // Ocean object

	// list of render objects
	std::vector<std::shared_ptr<Model>> m_RenderObjects;

    float m_lastY;
    float m_lastX;

	float m_lastFrame;
	float m_currentFrame;
	float m_deltaTime;
	
	float m_frameCount;

	glm::mat4 m_Model;
	glm::mat4 m_View;
	glm::mat4 m_Projection;

	// initializes GLFW and creates a GLFW window and OpenGL context, initializes GLEW
	bool Initialize();
	// Initiliazes GLFW and returns a pointer to the current GLFW window
	GLFWwindow* setupGLFWWindow(const uint32_t screenWidth, const uint32_t screenHeight);

    void processKeyboardInputs(); // process keyboard inputs

	void configureFrameTimes(); // configures deltaTime, lastFrame, and currentFrame
	void configureRenderMatrices(); // configures model, view, and projection matrices

	// deallocates glfw resources
	void glfwCleanUp(GLFWwindow* window);

};

//// GLFW callback functions //
//void glfwMouseCallback(double in_xPos, double in_yPos);
//void glfwScrollCallback(double xoffset, double yoffset);

