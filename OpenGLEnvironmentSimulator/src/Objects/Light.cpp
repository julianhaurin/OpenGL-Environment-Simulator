
#include "Light.h"

// returns light object with basic default values
Light getDefaultLight() {

	Light light;

	light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.position = glm::vec3(50.0f, 50.0f, 300.0f);

	return light;

}