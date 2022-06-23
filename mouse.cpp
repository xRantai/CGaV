#include "mouse.h"
#include "camera.h"

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

bool Mouse::firstMouse = true;

void Mouse::cursorPosCallback(GLFWwindow* window, double _x, double _y) {
	x = _x;
	y = _y;

	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	dx = x - lastX;
	dy = lastY - y; // y jest odwr�cony
	lastX = x;
	lastY = y;

	Camera::camera.updateCameraDirection(dx, dy);
}

double Mouse::getMouseX() {
	return x;
}

double Mouse::getMouseY() {
	return y;
}

double Mouse::getDX() {
	return dx;
}

double Mouse::getDY() {
	return dy;
}