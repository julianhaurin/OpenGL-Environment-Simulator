
#pragma once

#include <iostream>

#define GLEW_STATIC 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Initiliazes GLFW and GLEW and returns a pointer to the current GLFW window
GLFWwindow* setupOpenGLContext(const uint32_t screenHeight, const uint32_t screenWidth);

// GLFW callback functions
void glfwErrorCallback(int code, const char* description);
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);