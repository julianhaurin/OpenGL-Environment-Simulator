
// Holds light data and renders a light emitting source

#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ShaderHandlers/ShaderProgram.h"
#include "../Utility/Models/CubeModel.h"

struct LightData {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// spotlight attenuation parameters
	float constant;
	float linear;
	float quadratic;

	// constructors
	LightData() 
		: position(glm::vec3()),
		  ambient(glm::vec3()),
		  diffuse(glm::vec3()),
		  specular(glm::vec3()),
		  constant(1.0f),
		  linear(0.25f),
		  quadratic(0.15f)
	{}

	LightData(
		const glm::vec3 in_position,
		const glm::vec3 in_ambient,
		const glm::vec3 in_diffuse,
		const glm::vec3 in_specular,
		const float in_constant = 1.0f,
		const float in_linear = 0.45f,
		const float in_quadratic = 0.25f
	) 
	  : position(in_position), 
		ambient(in_ambient),
		diffuse(in_diffuse),
		specular(in_specular),
		constant(in_constant),
		linear(in_linear),
		quadratic(in_quadratic)
	{}

};

class Light
{
public:

	Light(const LightData in_lightData = getDefaultLightData());
	~Light();

	static LightData getDefaultLightData(); // returns LightData object with default values

	inline LightData getLightData() const { return m_lightData; }

	void Bind();
	void Render(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat); // renders object

private:

	LightData m_lightData;
	ShaderProgram m_ShaderProgram;

	const std::vector<float> m_Vertices;

	// OpenGL object IDs
	GLuint m_VBO;
	GLuint m_VAO;

	void Initialize(); // initializes object data and OpenGL structs
	void configureShader(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat);
	

};