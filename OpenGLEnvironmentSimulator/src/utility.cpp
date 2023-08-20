// Utility functions for OpenGL setup

#include "utility.h"


// Initiliazes GLFW and returns a pointer to the current GLFW window
GLFWwindow* setupGLFWWindow(const uint32_t screenWidth, const uint32_t screenHeight) {

    if (glfwInit() == GLFW_FALSE) {
        std::cout << "[J] - ERROR: GLFW failed to initialize\n";
        return nullptr;
    }

    // window specifications //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Environment Simulator", NULL, NULL);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: GLFW window failed to initialize\n";
        glfwTerminate();
        return nullptr;
    }

    // glfw callback functions
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor
    glEnable(GL_DEPTH_TEST);

    return window;

}

void glfwErrorCallback(int code, const char* description) {
    std::cout << "[J] - ERROR: GLFW error reported: " << description << std::endl;

}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

}

void glfwCleanUp(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();

}






//void glfwMouseCallback(GLFWwindow* in_Window, double in_xPos, double in_yPos) {
//    const float pPos = static_cast<float>(in_xPos);
//    const float yPos = static_cast<float>(in_yPos);
//
//    float xOffset = xPos - lastX;
//    float yOffset = lastY - yPos;
//
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}

