
// Model class

#include "Model.h"


// Public Methods //

Model::Model(const std::string in_objFile, const Material in_material, const std::string in_texturePath, const float in_sizeMultiplyer)
	: m_objFilePath(in_objFile), 
	  m_ShaderProgram(ShaderProgram("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs")),
	  m_VBO(0), m_EBO(0), m_VAO(0), m_vertexPositionMultiplyer(in_sizeMultiplyer),
	  m_Texture(in_texturePath), m_Material(in_material)
{
	assert(in_sizeMultiplyer >= 0);

	Light light;
	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.position = glm::vec3(00.0f, 00.0f, 300.0f);
	m_Light = light;

	loadObjData();
	setUpModel();
}

Model::~Model() {

}

void Model::Bind() {

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	m_Texture.Bind();

}

void Model::Render(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_ViewPos) {

	m_ShaderProgram.UseProgram();
	m_ShaderProgram.SetMat4("u_Model", in_ModelMat);
	m_ShaderProgram.SetMat4("u_View", in_ViewMat);
	m_ShaderProgram.SetMat4("u_Projection", in_ProjeMat);

	m_ShaderProgram.SetVec3("u_ObjectColor", glm::vec3(0.1f, 0.9f, 0.2f));

	// lighting
	m_ShaderProgram.SetVec3("u_Light.ambient", m_Light.ambient);
	m_ShaderProgram.SetVec3("u_Light.diffuse", m_Light.diffuse);
	m_ShaderProgram.SetVec3("u_Light.specular", m_Light.specular);
	m_ShaderProgram.SetVec3("u_Light.position", m_Light.position);

	m_ShaderProgram.SetVec3("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	//m_ShaderProgram.SetVec3("u_LightPosition", glm::vec3(200.0f, 200.0f, 300.0f));
	m_ShaderProgram.SetVec3("u_ViewPosition", in_ViewPos);

	// material 
	m_ShaderProgram.SetVec3("u_Material.ambient", m_Material.ambient);
	m_ShaderProgram.SetVec3("u_Material.diffuse", m_Material.diffuse);
	m_ShaderProgram.SetVec3("u_Material.specular", m_Material.specular);
	m_ShaderProgram.SetFloat("u_Material.ambient", m_Material.shininess);

	Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_VertexData.size() / 3);
	//glDrawElements(GL_TRIANGLES, m_IndexData.size() * 3, GL_UNSIGNED_INT, 0);
}

// Private Methods //

bool Model::loadObjData() {

	tinyobj::ObjReader objReader;

	tinyobj::ObjReaderConfig objReaderConfig;
	objReaderConfig.mtl_search_path = "./assets/textures/";

	// reads file data
	bool readStatus = objReader.ParseFromFile(m_objFilePath, objReaderConfig);

	// handles error messages
	if (readStatus == false) {
		// prints error message
		std::cout << "[J] ERROR - TINYOBJLOADER: unable to read from .obj file " << m_objFilePath << ": ";
		std::cout << objReader.Error() << std::endl;
		return false;

	}

	// handles warning messages
	if (!objReader.Warning().empty()) {
		std::cout << "[J] WARNING - TINYOBJLOADER: warning when reading obj data from file " << m_objFilePath << ": ";
		std::cout << objReader.Warning() << std::endl;
	}

	// loads data into containers //
	const tinyobj::attrib_t attrib = objReader.GetAttrib();
	const std::vector<tinyobj::shape_t> shapes = objReader.GetShapes();
	const std::vector<tinyobj::material_t> materials = objReader.GetMaterials();

	// populate class data //

	// loop over shapes
	for (size_t shapeIdx = 0; shapeIdx < shapes.size(); shapeIdx++) {
		size_t idxOffset = 0;

		// loop over faces
		for (size_t faceIdx = 0; faceIdx < shapes[shapeIdx].mesh.num_face_vertices.size(); faceIdx++) {
			size_t faceVertices = size_t(shapes[shapeIdx].mesh.num_face_vertices[faceIdx]);

			// loop over vertices in face
			for (size_t vertIdx = 0; vertIdx < faceVertices; vertIdx++) {

				tinyobj::index_t index = shapes[shapeIdx].mesh.indices[idxOffset + vertIdx];

				// VBO data //

				// vertex position
				m_VertexData.push_back(attrib.vertices[3 * (size_t)index.vertex_index] * m_vertexPositionMultiplyer); // + 0
				m_VertexData.push_back(attrib.vertices[3 * (size_t)index.vertex_index + 1] * m_vertexPositionMultiplyer);
				m_VertexData.push_back(attrib.vertices[3 * (size_t)index.vertex_index + 2] * m_vertexPositionMultiplyer);
				
				// EBO data
				m_IndexData.push_back(index);
	
				// normal data
				if (index.normal_index >= 0) {
					m_VertexData.push_back(attrib.normals[3 * size_t(index.normal_index)]); // + 0
					m_VertexData.push_back(attrib.normals[3 * size_t(index.normal_index) + 1]);
					m_VertexData.push_back(attrib.normals[3 * size_t(index.normal_index) + 2]);
				}

				if (index.texcoord_index >= 0) {
					m_VertexData.push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 0]);
					m_VertexData.push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 1]);

				}
			}

			idxOffset += faceVertices;

			//shapes[shapeIdx].mesh.material_ids[faceIdx];

		}
	}

	std::cout << "[J] - Successfully loaded model obj data! \n";

	return true;

}

void Model::setUpModel() {

	// generate and bind grid VBO, VAO, and EBO
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	// vbo set up
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_cubeVertices[0]) * m_cubeVertices.size(), &m_cubeVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_VertexData[0]) * m_VertexData.size(), &m_VertexData[0], GL_STATIC_DRAW);

	// ebo set up
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IndexData[0]) * m_IndexData.size(), &m_IndexData[0], GL_STATIC_DRAW);

	// VAO set up
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(m_VertexData[0]) * 8, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(m_VertexData[0]) * 8, (GLvoid*)(3 * sizeof(m_VertexData[0])));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(m_VertexData[0]) * 8, (GLvoid*)(6 * sizeof(m_VertexData[0])));

	//unsigned int diffuseMap = loadTexture("../assets/textures/containerDiffuseMap.png");

}