
#pragma once

// Notes:
// very simple for now, just trying to abstract info from main file into 
// cohesive element - lots of optimizing and updating to do
//
// should the opengl context (glfw/glew) be created within the engine or outside?
// fix error handling in inititalize function ***
//
// glfw callback function workaround stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
//

#include <iostream>
#include <cstdint>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera/Camera.h"
#include "Renderable/Ocean.h"
#include "utility.h"

// represents an OpenGL context and scene
class Engine 
{
public:
	// initial window dimensions //
	static const uint32_t ScreenHeight;
	static const uint32_t ScreenWidth;

	Engine();
	~Engine();

	// render loop
	void Run();

private:
    GLFWwindow* m_Window;
	Camera m_Camera;
	std::vector<Ocean> m_Objects;

    float m_lastY;
    float m_lastX;

	float m_deltaTime;
	float m_lastFrame;

	// initializes GLFW and creates a GLFW window and OpenGL context, initializes GLEW
	bool Initialize();
	// Initiliazes GLFW and returns a pointer to the current GLFW window
	GLFWwindow* setupGLFWWindow(const uint32_t screenWidth, const uint32_t screenHeight);

    // process keyboard inputs
    void processKeyboardInputs();

	// deallocates glfw resources
	void glfwCleanUp(GLFWwindow* window);

};


//// GLFW callback functions //
//void glfwMouseCallback(double in_xPos, double in_yPos);
//void glfwScrollCallback(double xoffset, double yoffset);