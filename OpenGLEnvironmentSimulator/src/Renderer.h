
// Notes:
// research batch rendering techniques and
// standard for rendering multiple objects
// youtube.com/watch?v=Th4huqR77rI
//
// glfw callback function workaround 
// stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
// 

#pragma once

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
#include "SkyBox/SkyBox.h"

#include "Objects/Light.h"
#include "Objects/Model.h"
#include "Objects/Material.h"

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
	
	void Run(); // initiates render loop
	void addModel(std::shared_ptr<Model> in_model_p); // to m_RenderObjects

private:
    GLFWwindow* m_Window; // OpenGL window abstraction
	Camera m_Camera; // camera abstraction

	// light source of scene
	std::vector<std::shared_ptr<Light>> m_LightSources;

	// list of render objects
	std::vector<std::shared_ptr<Model>> m_RenderObjects;

	// custom objects //
	std::shared_ptr<ShadowMap> m_ShadowMap; // Shadow map object
	std::shared_ptr<Ocean> m_Ocean; // Ocean object

	// rendering matrices //
	glm::mat4 m_Model;
	glm::mat4 m_View;
	glm::mat4 m_Projection;

	// frame and timing data //
	float m_lastY;
	float m_lastX;

	float m_lastFrame;
	float m_currentFrame;
	float m_deltaTime;

	float m_frameCount;

	// Methods //

	void processKeyboardInputs(); // process keyboard inputs

	void configureFrameTimes(); // configures deltaTime, lastFrame, and currentFrame
	void configureRenderMatrices(); // configures model, view, and projection matrices

	// initializes GLFW and creates a GLFW window and OpenGL context, initializes GLEW
	bool Initialize();

	// Initiliazes GLFW and returns a pointer to the current GLFW window
	GLFWwindow* setupGLFWWindow(const uint32_t screenWidth, const uint32_t screenHeight);
	void glfwCleanUp(GLFWwindow* window); // deallocates glfw resources
	 
};

// GLFW callback functions //
//void glfwMouseCallback(double in_xPos, double in_yPos);
//void glfwScrollCallback(double xoffset, double yoffset);

