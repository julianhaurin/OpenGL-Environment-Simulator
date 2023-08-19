
#pragma once

#include <glm/glm.hpp>

enum CameraMovement {
	Forwards,
	Backwards,
	Left,
	Right,
};

class Camera 
{
public: 

	const static float initSpeed;
	const static float initMouseSensitivity;
	const static float initZoom;

	Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch);

	void ProcessKeyboardInput(CameraMovement in_Direction, float deltaTime);
	void ProcessMouseInput(float in_xOffset, float in_yOffset, bool in_ConstrainPitch);
	void ProcessMouseScroll(float in_yOffset);

private:

	glm::vec3 m_Position;
	glm::vec3 m_WorldUp;

	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_Front;

	float m_Yaw;
	float m_Pitch;

	float m_Speed;
	float m_MouseSensitivity;
	float m_Zoom;

	void updateCameraVectors();
};