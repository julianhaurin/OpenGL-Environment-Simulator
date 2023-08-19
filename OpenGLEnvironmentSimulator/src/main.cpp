// OpenGL Environment Simulator - Julian Haurin - 8/17/23

#define GLEW_STATIC

#include <iostream>
#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "utility.h"

static const uint32_t SCREEN_HEIGHT = 600;
static const uint32_t SCREEN_WIDTH = 800;

void processInputs(GLFWwindow* window);

int main() 
{

    GLFWwindow* window = setupGLFWWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: Failed to initialize OpenGL with setupOpenGLContext(), returned nullptr window\n" << std::endl;
        glfwCleanUp(window);
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cout << "[J] - ERROR: GLEW failed to initialize: " << glewGetErrorString(glewStatus) << std::endl;
        glfwCleanUp(window);
        return EXIT_FAILURE;
    }

    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(util_CubeVertices), util_CubeVertices, GL_STATIC_DRAW);

    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //uint32_t EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    Shader shaderProgram("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs");
    shaderProgram.CompileShaders();
    shaderProgram.UseProgram();



    // render loop
    while (glfwWindowShouldClose(window) == false) 
    {
        processInputs(window);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //view = glm::lookAt(cameraPos, cameraPos, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        shaderProgram.SetMat4("model", model);
        shaderProgram.SetMat4("view", view);
        shaderProgram.SetMat4("projection", projection);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.2f, 0.3f, 1.0f); // background color

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shaderProgram.SetFloat("u_Color", greenValue);

        shaderProgram.UseProgram();
        glBindVertexArray(VAO);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwCleanUp(window);

	return EXIT_SUCCESS;

}

void processInputs(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}


