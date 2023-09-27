
#include "Camera.h"

const float Camera::initSpeed = 25.0f;
const float Camera::initMouseSensitivity = 25.0f;
const float Camera::initZoom = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
	: m_Position(position), m_WorldUp(worldUp),
	  m_Right(glm::vec3(1.0f, 0.0f, 0.0f)),
	  m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
	  m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	  m_Yaw(yaw), m_Pitch(pitch),
	  m_Speed(initSpeed), m_MouseSensitivity(initMouseSensitivity), m_Zoom(initZoom)
{
	updateCameraVectors();
}

void Camera::ProcessKeyboardInput(const CameraMovement in_Direction, const float deltaTime) {
	const float velocity = m_Speed * deltaTime;

	if (in_Direction == Forwards)
		m_Position += m_Front * velocity;
	else if (in_Direction == Backwards)
		m_Position -= m_Front * velocity;
	else if (in_Direction == Left)
		m_Position -= m_Right * velocity;
	else if (in_Direction == Right)
		m_Position += m_Right * velocity;

	if (in_Direction == Up)
		m_Position += m_Up * velocity;
	if (in_Direction == Down)
		m_Position -= m_Up * velocity;

	if (in_Direction == YawLeft)
		m_Yaw -= velocity;
	else if (in_Direction == YawRight)
		m_Yaw += velocity;
	else if (in_Direction == PitchUp)
		m_Pitch += velocity;
	else if (in_Direction == PitchDown)
		m_Pitch -= velocity;

	// constrains camera pitch angle
	if (m_Pitch > 89.9f)
		m_Pitch = 89.9f;
	if (m_Pitch < -89.9f)
		m_Pitch = -89.9f;

	updateCameraVectors();

}

void Camera::ProcessMouseInput(const float in_xOffset, const float in_yOffset, const bool in_ConstrainPitch) {
	
	m_Yaw += in_xOffset * m_MouseSensitivity;
	m_Pitch += in_yOffset * m_MouseSensitivity;

	if (in_ConstrainPitch == true) {
		if (m_Pitch > 89.9f)
			m_Pitch = 89.9f;
		if (m_Pitch < -89.9f)
			m_Pitch = -89.9f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float in_yOffset) {
	m_Zoom -= in_yOffset;
	if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom > 45.0f)
		m_Zoom = 45.0f;
}

glm::mat4 Camera::CalculateViewMatrix() const {
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

float Camera::getZoom() const { 
	return m_Zoom;

}

glm::vec3 Camera::getWorldPos() const {
	return m_Position;
}

// recalculates camera member variables when needed
void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

}