#ifndef CAMERA_H
#define CAMERA_H

#include <model.h>

enum class CameraDirection {
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	static Camera camera;

	RigidBody rb;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	glm::vec3 worldUp;

	float yaw; // x-axis
	float pitch; // y-axis
	float speed;

	Camera(glm::vec3 position);

	void updateCameraDirection(double dx, double dy); // moving mouse
	void updateCameraPos(CameraDirection direction, double dt); // keyboard input

	glm::mat4 getViewMatrix();

private:
	void updateCameraVectors();
};

#endif