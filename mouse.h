#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h>

class Mouse {
public:
	static void cursorPosCallback(GLFWwindow* window, double _x, double _y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	
	static double getMouseX();
	static double getMouseY();

	static double getDX();
	static double getDY();

	static bool button(int key);
	static bool buttonChanged(int key);
	static bool buttonWentUp(int key);
	static bool buttonWentDown(int key);

private:
	static double x;
	static double y;

	static double lastX;
	static double lastY;

	static double dx;
	static double dy;

	static bool firstMouse;

	static bool buttons[];
	static bool buttonsChanged[];
};

#endif
