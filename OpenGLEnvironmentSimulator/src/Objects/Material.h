
#pragma once

#include "glm/glm.hpp"

// Notes:
// dubious OOD

// phong lighting parameters for specific material
struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

Material getMaterialEmerald();

