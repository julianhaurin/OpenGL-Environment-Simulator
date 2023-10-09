
#include "SkyBox.h"

// type hardcoded for now
const std::vector<std::string> SkyBox::s_imageFaces =
{
    "right.bmp",
    "left.bmp",
    "top.bmp",
    "bottom.bmp",
    "front.bmp",
    "back.bmp"
};

const std::vector<float> SkyBox::s_skyBoxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f

};

// Public Methods //

SkyBox::SkyBox(const std::string in_textureFolderPath) 
	: m_textureFolderPath(in_textureFolderPath), 
      m_ShaderProgram(ShaderProgram("./shaders/skyboxVertShader.glsl", "./shaders/skyboxFragShader.glsl")),
      m_textureID(0)
{

    setupSkybox();
    loadAndGenSkyBoxTex();

    m_ShaderProgram.UseProgram();
    m_ShaderProgram.SetInt("skybox", 0);

}

void SkyBox::Bind() {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
}

void SkyBox::Render(glm::mat4 in_viewMat, const glm::mat4 in_projMat) {

    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    m_ShaderProgram.UseProgram();

    in_viewMat = glm::mat4(glm::mat3(in_viewMat)); // remove translation from the view matrix
    m_ShaderProgram.SetMat4("view", in_viewMat);
    m_ShaderProgram.SetMat4("projection", in_projMat);

    Bind();

    glActiveTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS); // sets depth function back to default value

}



// Private Methods //

bool SkyBox::loadAndGenSkyBoxTex() {

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    for (unsigned int i = 0; i < s_imageFaces.size(); i++)
    {

        const std::string currentPath = m_textureFolderPath + s_imageFaces[i];

        unsigned char* data = stbi_load(currentPath.c_str(), &m_width, &m_height, &m_nrChannels, 0);
        if (data == nullptr) {
            std::cout << "[J] ERROR - Unable to load skybox images at image: " << s_imageFaces[i] << std::endl;
            stbi_image_free(data);
            return false;
        }
        
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::cout << "[J] - Successfully loaded skybox images from root path of: " << m_textureFolderPath << "\n";
    return true;


}

void SkyBox::setupSkybox() {

    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_skyBoxVertices), &s_skyBoxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

