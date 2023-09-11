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


