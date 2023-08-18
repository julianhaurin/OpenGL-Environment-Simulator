// OpenGL Environment Simulator - Julian + Alex Haurin - 8/17/23

#define GLEW_STATIC

#include <iostream>
#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utility.h"

static const uint32_t SCREEN_HEIGHT = 600;
static const uint32_t SCREEN_WIDTH = 800;


int main() 
{

    GLFWwindow* window = setupOpenGLContext(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: Failed to initialize OpenGL with setupOpenGLContext()\n";
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor
    glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);

    // render loop
    while (glfwWindowShouldClose(window) == false) {

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.2f, 0.3f, 1.0f); // background color


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwDestroyWindow(window);
    glfwTerminate();
	return EXIT_SUCCESS;

}


