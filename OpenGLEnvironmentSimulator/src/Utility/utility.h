
#pragma once

#include <iostream>

#define GLEW_STATIC 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

// GLFW callback functions
void glfwErrorCallback(int code, const char* description);
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);

// prints all current OpenGL errors
void printOpenGLErrors();

// loads texture data with stb_image
//uint32_t loadTexture(std::string in_path);


