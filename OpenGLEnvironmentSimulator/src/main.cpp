// OpenGL Environment Simulator - Julian + Alex Haurin - 8/17/23

#include <iostream>
#include <cstdint>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utility.h"


static const uint32_t SCREEN_HEIGHT = 600;
static const uint32_t SCREEN_WIDTH = 800;

void glfwErrorCallback(int code, const char* description) {
    std::cout << "[J] - ERROR: GLFW error reported: " << description << std::endl;

}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

}

int main() 
{

    GLFWwindow* window = setupOpenGLContext(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: Failed to initialize OpenGL with setupOpenGLContext()\n";
    }

    // glfw callback functions
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor

    // render loop



    glfwDestroyWindow(window);
    glfwTerminate();
	return EXIT_SUCCESS;

}


