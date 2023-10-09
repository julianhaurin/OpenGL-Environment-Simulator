
#include "Light.h"

// Public Methods //

Light::Light(const LightData in_lightData) 
	: m_lightData(in_lightData),
	  m_ShaderProgram(ShaderProgram("./shaders/lightSourceVert.glsl", "./shaders/lightSourceFrag.glsl")),
	  m_Vertices(CubeModel::cubeVertices),
	  m_VBO(0), m_VAO(0)
{
	Initialize();
}

Light::~Light() 
{

	glDeleteVertexArrays(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
	//glDeleteBuffers(1, &m_EBO);

}

// binds OpenGL objects
void Light::Bind() {

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	m_ShaderProgram.UseProgram();

}

void Light::Render(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat) {

	Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size() / 8);

}

// Private Methods //

void Light::Initialize() {

	// generate and bind grid VBO, VAO, and EBO
	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	// VBO set up //
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices[0]) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

	// EBO set up //
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IndexData[0]) * m_IndexData.size(), &m_IndexData[0], GL_STATIC_DRAW);

	// VAO set up //
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(m_Vertices[0]) * 8, (GLvoid*)0); // position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(m_Vertices[0]) * 8, (GLvoid*)(3 * sizeof(m_Vertices[0]))); // normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(m_Vertices[0]) * 8, (GLvoid*)(6 * sizeof(m_Vertices[0]))); // texture coords

}

void Light::configureShader(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat) {

	m_ShaderProgram.UseProgram();

	// rendering matrices //
	m_ShaderProgram.SetMat4("u_Model", in_ModelMat);
	m_ShaderProgram.SetMat4("u_View", in_ViewMat);
	m_ShaderProgram.SetMat4("u_Projection", in_ProjeMat);

}

// returns light object with basic default values
LightData Light::getDefaultLightData() {

	LightData light;

	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.position = glm::vec3(50.0f, 50.0f, 300.0f);

	return light;

}