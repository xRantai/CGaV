#include "camera.h"

Camera::Camera(glm::vec3 position) 
	: cameraPos(position), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(0.0f), pitch(0.0f), speed(2.5f), zoom(50.0f), cameraFront(glm::vec3(0.0f, 0.0f, 0.0f)) {
	updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy) {
	yaw += dx;
	pitch += dy;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	updateCameraVectors();
}
void Camera::updateCameraPos(CameraDirection direction, double dt) {
	float velocity = (float)dt * speed;

	switch (direction) {
	case CameraDirection::FORWARD:
		cameraPos += cameraFront * velocity;
		break;
	case CameraDirection::BACKWARD:
		cameraPos -= cameraFront * velocity;
		break;
	case CameraDirection::RIGHT:
		cameraPos += cameraRight * velocity;
		break;
	case CameraDirection::LEFT:
		cameraPos -= cameraRight * velocity;
		break;
	case CameraDirection::UP:
		cameraPos += worldUp * velocity;
		break;
	case CameraDirection::DOWN:
		cameraPos -= worldUp * velocity;
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

	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}