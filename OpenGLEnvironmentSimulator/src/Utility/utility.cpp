// Utility functions for OpenGL setup

#include "utility.h"

// prints GLFW errors 
void glfwErrorCallback(int code, const char* description) {
    std::cout << "[J] - ERROR: GLFW error reported: " << description << std::endl;

}

// resizes OpenGL viewport based on window size
void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

}

// prints all current OpenGL errors
void printOpenGLErrors() {
	GLenum currentError = glGetError();
	while (currentError != GL_NO_ERROR) {
		std::cout << "[J] ERROR - OPENGL: OpenGL error detected: " << currentError << std::endl;
		currentError = glGetError();
	}

}


