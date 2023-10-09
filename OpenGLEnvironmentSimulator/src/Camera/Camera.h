
#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// camera movement directions
enum CameraMovement {
	Forwards,
	Backwards,
	Left,
	Right,
	Up,
	Down,
	YawRight,
	YawLeft,
	PitchUp,
	PitchDown,

};

class Camera 
{
public: 

	// initial camera variables //
	const static float initSpeed;
	const static float initMouseSensitivity;
	const static float initZoom;

	Camera(glm::vec3 position, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

	// processes keyboard inputs from glfw functions
	void ProcessKeyboardInput(const CameraMovement in_Direction, const float deltaTime);
	void ProcessMouseInput(const float in_xOffset, const float in_yOffset, const bool in_ConstrainPitch = true);
	void ProcessMouseScroll(const float in_yOffset);

	glm::mat4 CalculateViewMatrix() const;

	// getters
	inline float getZoom() const { return m_Zoom; }
	inline glm::vec3 getWorldPos() const { return m_Position; }

private:

	// positional vectors //
	glm::vec3 m_Position; // world position
	glm::vec3 m_WorldUp;

	glm::vec3 m_Right;
	glm::vec3 m_Up;
	glm::vec3 m_Front;

	// movement variables //
	float m_Yaw;
	float m_Pitch;

	float m_Speed; // movement speed
	float m_MouseSensitivity;
	float m_Zoom;

	// recalculates positional vectors
	void updateCameraVectors();

};