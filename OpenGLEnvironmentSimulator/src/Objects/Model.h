
// Represents an renderable object with a single VBO, EBO, and VAO

// Notes:
// obj material config path hardcoded to ./assets/textures/ remember this being a bug before but idk if its always necessary
// allow making models smaller

#pragma once

#include <iostream>
#include <string>
#include <cassert>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <tinyobj/tiny_obj_loader.h>	

#include "../ShaderHandlers/ShaderProgram.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "../Utility/utility.h"

//#include "../Utility/Models/CubeModel.h"

// represents data of one vertex
//struct VertexData {
//	glm::vec3 pos; // xyz position of vertex
//	glm::vec3 norm; // direction of normal vector
//	//glm::vec3 texCoord; // texture coordinate
//};

class Model
{
public:

	// in_sizeMultiplyer controls size of object (makes it bigger)
	Model(const std::string in_objFile, const Material in_material, const std::string in_texturePath, const float in_sizeMultiplyer = 1);
	~Model();

	void Bind();
	void Render(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_ViewPos);


private:

	const std::string m_objFilePath;

	ShaderProgram m_ShaderProgram;

	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_VAO;

	std::vector<tinyobj::real_t> m_VertexData; // EBO data
	std::vector<tinyobj::index_t> m_IndexData; // EBO data

	Texture m_Texture;
	const Material m_Material;
	const float m_vertexPositionMultiplyer;

	Light m_Light;

	std::vector<float> m_cubeVertices;
	uint32_t m_diffuseMap;

	bool loadObjData();
	void setUpModel();

};