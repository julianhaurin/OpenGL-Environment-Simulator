// OpenGL Environment Simulator - Julian Haurin - 8/17/23

// Notes:
// version might not be right in shaders
// glfw callback function require global camera class - fix
// add up and down movements to camera class

#define GLEW_STATIC

#include <iostream>
#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/ShaderProgram.h"
#include "Camera.h"
#include "utility.h"
#include "Ocean.h"

static const uint32_t SCREEN_HEIGHT = 600;
static const uint32_t SCREEN_WIDTH = 800;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool firstMouse = true;

void processInputs(GLFWwindow* window);
void glfwMouseCallback(GLFWwindow* window, double in_xPos, double in_yPos);
void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastY = SCREEN_HEIGHT / 2.0f;
float lastX = SCREEN_WIDTH / 2.0f;

int main() 
{

    GLFWwindow* window = setupGLFWWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: Failed to initialize OpenGL with setupOpenGLContext(), returned nullptr window\n" << std::endl;
        glfwCleanUp(window);
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, glfwMouseCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cout << "[J] - ERROR: GLEW failed to initialize: " << glewGetErrorString(glewStatus) << std::endl;
        glfwCleanUp(window);
        return EXIT_FAILURE;
    }


    glEnableVertexAttribArray(0);

    ShaderProgram shaderProgram("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs");
    shaderProgram.CompileShaders();
    shaderProgram.UseProgram();

    Ocean ocean(16);
    ocean.Initialize();

    // render loop
    while (glfwWindowShouldClose(window) == false) 
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInputs(window);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.CalculateViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        shaderProgram.SetMat4("model", model);
        shaderProgram.SetMat4("view", view);
        shaderProgram.SetMat4("projection", projection);
        shaderProgram.UseProgram();

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // background color
        
        ocean.Update();

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwCleanUp(window);

	return EXIT_SUCCESS;

}

void processInputs(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboardInput(Forwards, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboardInput(Backwards, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboardInput(Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboardInput(Right, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboardInput(Up, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboardInput(Down, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

void glfwMouseCallback(GLFWwindow* window, double in_xPos, double in_yPos) {
    const float xPos = static_cast<float>(in_xPos);
    const float yPos = static_cast<float>(in_yPos);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseInput(xOffset, yOffset);
}

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));

}

//uint32_t VBO;
//glGenBuffers(1, &VBO);
//glBindBuffer(GL_ARRAY_BUFFER, VBO);
//glBufferData(GL_ARRAY_BUFFER, sizeof(util_CubeVertices), util_CubeVertices, GL_STATIC_DRAW);
//
//uint32_t VAO;
//glGenVertexArrays(1, &VAO);
//glBindVertexArray(VAO);
//
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);



