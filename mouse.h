#ifndef MOUSE_H
#define MOUSE_H

#include "camera.h"
#include <GLFW/glfw3.h>

class Mouse {
public:
	static void cursorPosCallback(GLFWwindow* window, double _x, double _y);

private:
	static double x;
	static double y;

	static double lastX;
	static double lastY;

	static double dx;
	static double dy;

	static bool firstMouse;
};

#endif