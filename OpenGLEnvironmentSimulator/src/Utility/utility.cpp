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

// prints all current OpenGL errors
void printOpenGLErrors() {
	GLenum currentError = glGetError();
	while (currentError != GL_NO_ERROR) {
		std::cout << "[J] ERROR - OPENGL: OpenGL error detected: " << currentError << std::endl;
		currentError = glGetError();
	}

}

// loads textures
//uint32_t loadTexture(std::string in_path) {
//
//    uint32_t textureID;
//    glGenTextures(1, &textureID);
//
//    int32_t width, height, nrComponents;
//    unsigned char* data = stbi_load(in_path.c_str(), &width, &height, &nrComponents, 0);
//    if (data != nullptr) {
//        GLenum format;
//        if (nrComponents == 1) format = GL_RED;
//        else if (nrComponents == 3) format = GL_RGB;
//        else if (nrComponents == 4) format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "[J] ERROR: Unable to load texure with path " << in_path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}


