// OpenGL Environment Simulator - Julian + Alex Haurin - 8/17/23

#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void print(std::string text) {
    std::cout << text;
}

int main() 
{

	std::cout << "Hello, World!";

    if (glfwInit() == GLFW_FALSE) {
        print("JH - GLFW failed to initialize\n");
        return -1;
    }
    if (!glewInit()) {
        print("JH - GLEW failed to initialize\n");
        return -1;
    }

	return 0;

}
