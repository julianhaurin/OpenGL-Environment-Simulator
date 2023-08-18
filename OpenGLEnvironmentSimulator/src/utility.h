
#pragma once

#include <iostream>

#define GLEW_STATIC 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Initiliazes GLFW and returns a pointer to the current GLFW window
GLFWwindow* setupGLFWWindow(const uint32_t screenHeight, const uint32_t screenWidth);

// processes key inputs from user
void processInputs(GLFWwindow* window);

// GLFW callback functions
void glfwErrorCallback(int code, const char* description);
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);