#include "camera.h"

Camera::Camera(glm::vec3 position)
	: worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(-90.0f),
	pitch(0.0f),
	speed(2.5f),
	cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
	rb.pos = position;
	//rb.acceleration = Environment::gravitationalAcceleration;
	updateCameraVectors();
}

// mouse movement
void Camera::updateCameraDirection(double dx, double dy) {
	yaw += dx / 2;
	pitch += dy / 2;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraPos(CameraDirection direction, double dt) {
	glm::vec3 front, right;

	/*
		Calculating the vectors parallel to XZ plane
	*/

	front.x = cos(glm::radians(yaw));
	front.y = 0;
	front.z = sin(glm::radians(yaw));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));

	switch (direction) {
	case CameraDirection::FORWARD:
		rb.velocity += front * speed;
		break;
	case CameraDirection::BACKWARD:
		rb.velocity -= front * speed;
		break;
	case CameraDirection::RIGHT:
		rb.velocity += right * speed;
		break;
	case CameraDirection::LEFT:
		rb.velocity -= right * speed;
		break;
	case CameraDirection::UP:
		rb.velocity += worldUp * 10.0f;
		break;
	}
	
	rb.update(dt);
	rb.velocity = glm::vec3(0, rb.velocity.y, 0);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(rb.pos, rb.pos + cameraFront, cameraUp);
}

void Camera::setScene(std::vector<Model>& vec) {
	scene = vec;
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