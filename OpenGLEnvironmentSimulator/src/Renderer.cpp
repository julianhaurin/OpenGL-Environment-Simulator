
#include "Renderer.h"

const uint32_t Renderer::ScreenHeight = 600;
const uint32_t Renderer::ScreenWidth = 800;

// Public Methods ------------------------------ //

Renderer::Renderer()
    : m_Camera(Camera(glm::vec3(0.0f, 5.0f, 5.0f))),
    m_deltaTime(0.0f), m_lastFrame(0.0f),
    m_lastY(ScreenHeight / 2.0f), m_lastX(ScreenWidth / 2.0f)
{

    // initializes GLFW, creates OpenGL context and GLFW window, initializes GLEW //
    if (Initialize() != true) {
        std::cout << "[J] - ERROR: Renderer failed to initialize \n";
        return; // lol bruh
    }
    std::cout << "[J] - Renderer successfully initialized! \n\n";

    Ocean m_Ocean = Ocean(16, 0.0025f, glm::vec2(32.0f, 32.0f), 32);
    m_Objects.push_back(m_Ocean);

}

Renderer::~Renderer()
{
    glfwCleanUp(m_Window);
}

// render loop
void Renderer::Run() {

    std::cout << "[J] - Beginning render loop... \n";
    glm::vec3 lightPos(0.0f, -10.0f, 50.0f);

    while (glfwWindowShouldClose(m_Window) == false)
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        processKeyboardInputs();
        
        // model, view, and projection matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_Camera.CalculateViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(
            m_Camera.getZoom()), 
            static_cast<float>(ScreenWidth) / static_cast<float>(ScreenWidth), 
            0.1f, 255.0f
        );

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f); // background color

        m_Objects[0].Render(currentFrame, model, view, projection, lightPos);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();

    }
}

// Private Methods ----------------------------- //

// initializes GLFW and creates a GLFW window and OpenGL context, initializes GLEW
bool Renderer::Initialize() {

    std::cout << "[J] - Initializing Renderer... \n";

    // initializes GLFW and returns a valid GLFWwindow* on success
    m_Window = setupGLFWWindow(ScreenWidth, ScreenHeight);
    if (m_Window == nullptr) {
        std::cout << "[J] - ERROR: Failed to initialize OpenGL with setupOpenGLContext(), returned nullptr window\n" << std::endl;
        glfwCleanUp(m_Window);
        return false;
    }

    glfwMakeContextCurrent(m_Window);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cout << "[J] - ERROR: GLEW failed to initialize: " << glewGetErrorString(glewStatus) << std::endl;
        glfwCleanUp(m_Window);
        return false;
    }

    // glfw callback functions //
    glfwSetErrorCallback(glfwErrorCallback);
    glfwSetFramebufferSizeCallback(m_Window, glfwFramebufferSizeCallback);
    // glfwSetCursorPosCallback(m_Window, glfwMouseCallback);
    // glfwSetScrollCallback(m_Window, glfwScrollCallback);

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor
    glEnable(GL_DEPTH_TEST);

    std::cout << "[J] - Successfully initialized GLEW! \n";
    return true;

}

// Initiliazes GLFW and returns a pointer to the current GLFW window
GLFWwindow* Renderer::setupGLFWWindow(const uint32_t screenWidth, const uint32_t screenHeight) {

    if (glfwInit() == GLFW_FALSE) {
        std::cout << "[J] - ERROR: GLFW failed to initialize\n";
        return nullptr;
    }

    std::cout << "[J] - Successfully initialized GLFW! \n";

    // glfw window specifications //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Environment Simulator", NULL, NULL);
    if (window == nullptr) {
        std::cout << "[J] - ERROR: GLFW window failed to initialize\n";
        glfwTerminate();
        return nullptr;
    }

    std::cout << "[J] - Successfully initialized GLFW window! \n";
    return window;

}

// processes keyboard inputs
void Renderer::processKeyboardInputs() {

    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_Window, true);
    }

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(Forwards, m_deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(Backwards, m_deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(Left, m_deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(Right, m_deltaTime);

    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(Up, m_deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(Down, m_deltaTime);

    if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(YawLeft, m_deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(YawRight, m_deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(PitchUp, m_deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_Camera.ProcessKeyboardInput(PitchDown, m_deltaTime);

    // solid and wireframe modes
    if (glfwGetKey(m_Window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(m_Window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

// deallocates glfw resources
void Renderer::glfwCleanUp(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();

}

//// GLFW callback functions //
//void Renderer::glfwMouseCallback(double in_xPos, double in_yPos) {
//    const float xPos = static_cast<float>(in_xPos);
//    const float yPos = static_cast<float>(in_yPos);
//
//    float xOffset = xPos - m_lastX;
//    float yOffset = m_lastY - yPos;
//
//    m_lastX = xPos;
//    m_lastY = yPos;
//
//    m_Camera.ProcessMouseInput(xOffset, yOffset);
//}
//
//void Renderer::glfwScrollCallback(double xoffset, double yoffset) {
//    m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
//
//}


