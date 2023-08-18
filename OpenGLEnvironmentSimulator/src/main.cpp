// OpenGL Environment Simulator - Julian Haurin - 8/17/23

#define GLEW_STATIC

#include <iostream>
#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "utility.h"

static const uint32_t SCREEN_HEIGHT = 600;
static const uint32_t SCREEN_WIDTH = 800;

int main() 
{

    GLFWwindow* window = setupGLFWWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: Failed to initialize OpenGL with setupOpenGLContext(), returned nullptr window\n" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor
    glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cout << "[J] - ERROR: GLEW failed to initialize: " << glewGetErrorString(glewStatus) << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // triangle rendering test
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    uint32_t EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    Shader shaderProgram("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs");
    shaderProgram.CompileShaders();
    shaderProgram.UseProgram();


    // render loop
    while (glfwWindowShouldClose(window) == false) 
    {
        processInputs(window);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.2f, 0.3f, 1.0f); // background color

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shaderProgram.SetFloat("u_Color", greenValue);

        shaderProgram.UseProgram();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();

	return EXIT_SUCCESS;

}
