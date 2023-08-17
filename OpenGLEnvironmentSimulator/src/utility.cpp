// Utility functions for OpenGL setup

#include "utility.h"


// Initiliazes GLFW and GLEW and returns a pointer to the current GLFW window
GLFWwindow* setupOpenGLContext(const uint32_t screenHeight, const uint32_t screenWidth) {

    if (glfwInit() == GLFW_FALSE) {
        std::cout << "[J] - ERROR: GLFW failed to initialize\n";
        return nullptr;
    }

    // glewInit() != GLEW_OK
    if (!glewInit()) {
        std::cout << "[J] - ERROR: GLEW failed to initialize\n";
        glfwTerminate();
        return nullptr;
    }

    // window specifications //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Environment Simulator", NULL, NULL);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: GLFW window failed to initialize\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    return window;

}