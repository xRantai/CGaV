/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"


float speed = 0;
float rotate = 0;
glm::vec3 pos = { 0.0,0.0,-5.0 }; //pozycja XYZ


//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obs³ugi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) { //Podstawowy model poruszania siê (bez kolizji)
		if (key == GLFW_KEY_LEFT) rotate -= PI/2; 
		if (key == GLFW_KEY_RIGHT) rotate += PI/2; 
		if (key == GLFW_KEY_UP) speed += 1; 
		if (key == GLFW_KEY_DOWN) speed -= 1; 
	
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) rotate += PI/2;
		if (key == GLFW_KEY_RIGHT) rotate -= PI/2;
		if (key == GLFW_KEY_UP) speed -= 1;
		if (key == GLFW_KEY_DOWN) speed += 1;
	}
}




//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Place any code here that needs to be executed once, at the program start************
	glClearColor(0, 0, 0, 1); //Set color buffer clear color
	glEnable(GL_DEPTH_TEST); //Turn on pixel depth test based on depth buffer
	glfwSetKeyCallback(window, key_callback);
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Place any code here that needs to be executed once, after the main loop ends************
}

//Drawing procedure
void drawScene(GLFWwindow* window, float angle) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Compute projection matrix
	glm::mat4 V = glm::lookAt(pos, glm::vec3(pos[0]+cos(angle)*1.0, pos[1], pos[2]+sin(angle)*1.0), glm::vec3(0.0f, 1.0f, 0.0f)); //Compute view matrix
	

	spLambert->use();//Aktywacja programu cieniuj¹cego
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); 
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));

	Models::teapot.drawSolid();

	glfwSwapBuffers(window); //Copy back buffer to the front buffer
}

int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Call initialization procedure

	//Main application loop
	float angle = 0.0;
	
	glfwSetTime(0); //clear internal timer
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		
		angle += rotate * glfwGetTime(); // Rotacja

		pos[0] += cos(angle) * speed * glfwGetTime(); // Prymitywny model poruszania siê bez kolizji
		pos[2] += sin(angle) * speed * glfwGetTime();

		glfwSetTime(0); //clear internal timer
		drawScene(window, angle); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
