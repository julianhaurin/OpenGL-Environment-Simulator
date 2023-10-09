
#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLFW callback functions
void glfwErrorCallback(int code, const char* description);
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);

// prints all current OpenGL errors
void printOpenGLErrors();


