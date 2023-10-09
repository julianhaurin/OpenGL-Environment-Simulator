
// Represents an renderable object with a VBO, EBO, and VAO
// loads data from .obj file

// Notes:
// obj material config path hardcoded to ./assets/textures/ remember this being a bug before but idk if its always necessary
// allow making models smaller
// fix default texture ***

#pragma once

#include <iostream>
#include <string>
#include <cassert>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <tinyobj/tiny_obj_loader.h>	

#include "../Utility/utility.h"
#include "../ShaderHandlers/ShaderProgram.h"

#include "Material.h"
#include "Texture.h"
#include "Light.h"

class Model
{
public:

	// in_sizeMultiplyer controls size of object
	Model(const std::string in_objFile, const float in_sizeMultiplyer = 1);
	Model(const std::string in_objFile, const Material in_material, const std::string in_texturePath, const float in_sizeMultiplyer = 1);
	~Model();

	// binds objects and all OpenGL objects within class
	void Bind(const bool bindTexture = true);

	// renders object (called each loop)
	void Render(
		glm::mat4 in_ModelMat, 
		glm::mat4 in_ViewMat, 
		glm::mat4 in_ProjeMat, 
		glm::vec3 in_ViewPos, 
		glm::mat4 in_LightSpaceMat, 
		const bool useModelShader = true
	);


private:

	const std::string m_objFilePath; // path to .obj file
	const float m_vertexPositionMultiplyer;

	ShaderProgram m_ShaderProgram; // shader program of object

	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_VAO;

	std::vector<tinyobj::real_t> m_VertexData; // EBO data
	std::vector<tinyobj::index_t> m_IndexData; // EBO data

	// model objects
	Texture m_Texture;
	Material m_Material;
	Light m_Light;

	// Methods //

	bool loadObjData(); // loads data from .obj file path into m_VertexData and m_IndexData (tinyobjloader)
	void setUpModel(); // initializes VBO, EBO, and VAO objects

	// configures shader data (uniforms)
	void configureShader(
		glm::mat4 in_ModelMat, 
		glm::mat4 in_ViewMat, 
		glm::mat4 in_ProjeMat, 
		glm::vec3 in_ViewPos
	);

};