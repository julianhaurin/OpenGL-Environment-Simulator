
// Model class

#include "Model.h"


// Public Methods //

Model::Model(const std::string in_objFile, const std::vector<std::shared_ptr<Light>> in_lightSources, const float in_sizeMultiplyer)
	: m_objFilePath(in_objFile), m_vertexPositionMultiplyer(in_sizeMultiplyer),
	  m_ShaderProgram(ShaderProgram("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs")),
	  m_VBO(0), m_EBO(0), m_VAO(0), 
	  m_LightSources(in_lightSources),
	  m_Material(getDefaultMaterial()),
	  m_Texture(Texture("./assets/textures/quack.png"))
{
	assert(m_vertexPositionMultiplyer >= 0);

	loadObjData();
	setUpModel();

}

Model::Model(
	const std::string in_objFile, 
	const std::vector<std::shared_ptr<Light>> in_lightSources,
	const Material in_material, 
	const std::string in_texturePath, 
	const float in_sizeMultiplyer
)
	: m_objFilePath(in_objFile), m_vertexPositionMultiplyer(in_sizeMultiplyer),
	  m_ShaderProgram(ShaderProgram("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs")),
	  m_VBO(0), m_EBO(0), m_VAO(0), 
	  m_LightSources(in_lightSources),
	  m_Material(getDefaultMaterial()),
	  m_Texture(Texture("./assets/textures/quack.png"))
{
	assert(in_sizeMultiplyer >= 0);

	loadObjData();
	setUpModel();
}

// deallocate OpenGL resources
Model::~Model() {

	glDeleteVertexArrays(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_EBO);
}

// binds OpenGL objects
void Model::Bind(const bool bindTexture) {

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	if (bindTexture) m_Texture.Bind();

	m_ShaderProgram.UseProgram();

}

void Model::Render(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_ViewPos, glm::mat4 in_LightSpaceMat, const bool useModelShader) {

	configureShader(in_ModelMat, in_ViewMat, in_ProjeMat, in_ViewPos); // don't run every loop
	Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_VertexData.size() / 3);
	// glDrawElements(GL_TRIANGLES, m_IndexData.size() * 3, GL_UNSIGNED_INT, 0);
	
}

// Private Methods //

// loads data into m_Vertices and m_Indices using tinyobjloader
bool Model::loadObjData() {

	tinyobj::ObjReader objReader;
	tinyobj::ObjReaderConfig objReaderConfig;
	objReaderConfig.mtl_search_path = "./assets/textures/";

	bool readStatus = objReader.ParseFromFile(m_objFilePath, objReaderConfig); // reads file data

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

	// VBO set up //
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_VertexData[0]) * m_VertexData.size(), &m_VertexData[0], GL_STATIC_DRAW);

	// EBO set up //
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IndexData[0]) * m_IndexData.size(), &m_IndexData[0], GL_STATIC_DRAW);

	// VAO set up //
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(m_VertexData[0]) * 8, (GLvoid*)0); // position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(m_VertexData[0]) * 8, (GLvoid*)(3 * sizeof(m_VertexData[0]))); // normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(m_VertexData[0]) * 8, (GLvoid*)(6 * sizeof(m_VertexData[0]))); // texture coords

}

void Model::configureShader(glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat, glm::vec3 in_ViewPos) {

	m_ShaderProgram.UseProgram();

	// rendering matrices //
	m_ShaderProgram.SetMat4("u_Model", in_ModelMat);
	m_ShaderProgram.SetMat4("u_View", in_ViewMat);
	m_ShaderProgram.SetMat4("u_Projection", in_ProjeMat);

	// material values //
	m_ShaderProgram.SetVec3("u_Material.ambient", m_Material.ambient);
	m_ShaderProgram.SetVec3("u_Material.diffuse", m_Material.diffuse);
	m_ShaderProgram.SetVec3("u_Material.specular", m_Material.specular);
	m_ShaderProgram.SetFloat("u_Material.shininess", m_Material.shininess);

	// lighting //
	for (int i = 0; i < m_LightSources.size(); i++) {
		LightData lightingData = m_LightSources[i]->getLightData();

		m_ShaderProgram.SetVec3("u_LightSources[" + std::to_string(i) + "].position", lightingData.position);
		m_ShaderProgram.SetVec3("u_LightSources[" + std::to_string(i) + "].ambient", lightingData.ambient);
		m_ShaderProgram.SetVec3("u_LightSources[" + std::to_string(i) + "].diffuse", lightingData.diffuse);
		m_ShaderProgram.SetVec3("u_LightSources[" + std::to_string(i) + "].specular", lightingData.specular);

		m_ShaderProgram.SetFloat("u_LightSources[" + std::to_string(i) + "].constant", lightingData.constant);
		m_ShaderProgram.SetFloat("u_LightSources[" + std::to_string(i) + "].linear", lightingData.linear);
		m_ShaderProgram.SetFloat("u_LightSources[" + std::to_string(i) + "].quadratic", lightingData.quadratic);

	}
	
	
	// other (fix) //
	m_ShaderProgram.SetVec3("u_ViewPosition", in_ViewPos);


}

