#include "camera.h"

Camera::Camera(glm::vec3 position)
	: cameraPos(position),
	worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(-90.0f),
	pitch(0.0f),
	speed(2.5f),
	cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	updateCameraVectors();
}

// mouse movement
void Camera::updateCameraDirection(double dx, double dy) {
	yaw += dx;
	pitch += dy;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraPos(CameraDirection direction, double dt) {
	float velocity = (float)dt * speed;
	glm::vec3 front, right;

	/*
		Calculating the vectors irrelevant to yaw
	*/

	front.x = cos(glm::radians(yaw));
	front.y = 0;
	front.z = sin(glm::radians(yaw));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));

	switch (direction) {
	case CameraDirection::FORWARD:
		cameraPos += front * velocity;
		break;
	case CameraDirection::BACKWARD:
		cameraPos -= front * velocity;
		break;
	case CameraDirection::RIGHT:
		cameraPos += right * velocity;
		break;
	case CameraDirection::LEFT:
		cameraPos -= right * velocity;
		break;
	}
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::updateCameraVectors() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}