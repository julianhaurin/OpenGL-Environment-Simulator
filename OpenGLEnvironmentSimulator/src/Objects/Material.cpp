
#include "Material.h"

Material getMaterialEmerald() {

	Material emeraldMaterial;
	emeraldMaterial.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
	emeraldMaterial.diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
	emeraldMaterial.specular = glm::vec3(0.633f, 0.727811f, 0.633f);
	emeraldMaterial.shininess = 0.6f;

	return emeraldMaterial;

}