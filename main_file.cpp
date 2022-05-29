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


float speed = 0;//[radians/s]

Models::Sphere sun(0.5, 36, 36);
Models::Sphere planet1(0.2, 36, 36);
Models::Sphere moon1(0.1, 36, 36);

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obs³ugi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed = -PI; //Je¿eli wciœniêto klawisz "w lewo" ustaw prêdkoœæ na -PI
		if (key == GLFW_KEY_RIGHT) speed = PI; //Je¿eli wciœniêto klawisz "w prawo" ustaw prêdkoœæ na PI
	
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) speed = 0;	
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


void planets1(float angle) {
	glm::mat4 Ms = glm::mat4(1.0f); //Sun model matrix is an identity matrix
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Ms));  //Load model matrix into shader program
	glUniform4f(spLambert->u("color"), 1, 1, 0, 1); //Sun is yellow
	sun.drawSolid(); //Draw sun


	glm::mat4 Mp1 = glm::rotate(Ms, angle, glm::vec3(0.0f, 1.0f, 0.0f)); //Planet's model matrix is a sun matrix multiplied by rotation...
	Mp1 = glm::translate(Mp1, glm::vec3(1.5f, 0.0f, 0.0f)); //...and translation matrix
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mp1));  //Load model matrix into shader program
	glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Planet is green
	planet1.drawSolid(); //Draw planet

	glm::mat4 Mk1 = glm::rotate(Mp1, angle, glm::vec3(0.0f, 1.0f, 0.0f)); //Moon's model matrix is a planet matrix multiplied by rotation...
	Mk1 = glm::translate(Mk1, glm::vec3(0.5f, 0.0f, 0.0f));//...and translation matrix
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk1));  //Load model matrix into shader program
	glUniform4f(spLambert->u("color"), 0.5, 0.5, 0.5, 1); //Moon is gray
	moon1.drawSolid(); //Draw moon
}

//Drawing procedure
void drawScene(GLFWwindow* window, float angle) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Compute projection matrix
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Compute view matrix

	spLambert->use();//Aktywacja programu cieniuj¹cego
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); 

		
	planets1(angle);
		

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
	float angle = 0; //declare variable for storing current rotation angle
	
	glfwSetTime(0); //clear internal timer
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		angle += speed * glfwGetTime(); //Compute an angle by which the object was rotated during the previous frame		
		glfwSetTime(0); //clear internal timer
		drawScene(window, angle); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
