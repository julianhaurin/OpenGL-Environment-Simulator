
#pragma once

// learn cpp oop bum

#include <glm/glm.hpp>

class Renderable
{
public:
	//virtual void Render() = 0;
	virtual void Render(const float time, glm::mat4 in_ModelMat, glm::mat4 in_ViewMat, glm::mat4 in_ProjeMat) = 0;

};