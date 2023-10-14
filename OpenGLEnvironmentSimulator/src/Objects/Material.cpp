
#include "Material.h"

Material getDefaultMaterial() {

	Material defaultMat;

	defaultMat.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	defaultMat.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	defaultMat.specular = glm::vec3(0.9f, 0.9f, 0.9f);
	defaultMat.shininess = 256.0f; // *** used in specular calculations - dont think 0 to 1 is good

	return defaultMat;

}

// custom emeral material
Material getMaterialEmerald() {

	Material emeraldMaterial;
	emeraldMaterial.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	emeraldMaterial.diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
	emeraldMaterial.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
	emeraldMaterial.shininess = 0.6f;

	return emeraldMaterial;

}