/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth model[1], Boston, MA  02110 - 1301  USA
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
#include "shaderprogram.h"
#include "model_loader.h"

#include "model.h"

#include "keyboard.h"
#include "mouse.h"
#include "camera.h"

Camera Camera::camera(glm::vec3(7.0f, 1.5f, 2.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::vector<Model> modelTemplates;
std::vector<Model> scene; // wszystkie renderowane modele


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void processInput(GLFWwindow* window, double dt) {
	/*
		Close window
	*/

	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	/*
		Keyboard movement
	*/

	if (Keyboard::key(GLFW_KEY_W)) {
		Camera::camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		Camera::camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		Camera::camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		Camera::camera.updateCameraPos(CameraDirection::LEFT, dt);
	}
}

void initModels() {
	/*
	
	for (int i = 0; i < 7; i++) { //ceiling
		for (int j = 0; j < 7; j++) {
			scene.push_back(Model(modelTemplates[1], glm::vec3(8.0f - 2.3f * i, 1.8f, 6.5f - 2.3f*j), float(0), glm::vec3(2.3f)));
		}
	}
	for (int i = 0; i < 7; i++) { //first floor floor
		for (int j = 0; j < 7; j++) {
			if (i == 3 && j == 4) { continue; }
			scene.push_back(Model(modelTemplates[1], glm::vec3(8.0f - 2.3f * i, 0.0f, 6.5f - 2.3f * j), float(0), glm::vec3(2.3f)));
		}
	}

	for (int i = 0; i < 7; i++) { //second floor floor
		for (int j = 0; j < 7; j++) {
			scene.push_back(Model(modelTemplates[1], glm::vec3(8.0f - 2.3f * i, -1.8f, 6.5f - 2.3f * j), float(0), glm::vec3(2.3f)));
		}
	}

	*/
	scene.push_back(Model(modelTemplates[2], glm::vec3(1.1f, -0.15f, -2.7f), float(0), glm::vec3(1.2f))); //hole model

	for (int i = 0; i < 4; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(8.25f - i*2.25f, 0.375f, 3.0f), float(0), glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(0.075f, 0.375f, 4.125f), float(PI/2), glm::vec3(1.5f)));

	for (int i = 0; i < 3; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(1.2f + i * 2.25f, 0.375f, 5.55f), float(PI), glm::vec3(1.5f)));
	}
	
	scene.push_back(Model(modelTemplates[0], glm::vec3(0.3f, 0.375f, 2.145f), float(-PI / 2), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-0.825f, 0.375f, 0.72f), float(0), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.075f, 0.375f, 0.72f), float(0), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.5f, 0.375f, 1.845f), float(PI/2), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.5f, 0.375f, 4.095f), float(PI / 2), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.25f, 0.375f, 4.095f), float(PI / 2), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.25f, 0.375f, 6.345f), float(PI / 2), glm::vec3(1.5f)));

	for (int i = 0; i < 5; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-1.125f + 2.25f*i, 0.375f, 7.77f), float(PI), glm::vec3(1.5f)));
	}

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(9.3f, 0.375f, 6.645f-2.25f*i), float(-PI / 2), glm::vec3(1.5f)));
	}

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(8.175f-2.25f*i, 0.375f, -8.28f), float(0), glm::vec3(1.5f)));
	}

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-6.75f, 0.375f, -7.155f + 2.25f * i), float(PI/2), glm::vec3(1.5f)));
	}

	for (int i = 0; i < 2; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-5.625f + 2.25f * i, 0.375f, 7.77f), float(PI), glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(8.175f, 0.375f, 0.72f), float(0), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(5.925f, 0.375f, 0.72f), float(0), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(4.725f, 0.375f, -0.135f), float(-PI/2), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(5.575f, 0.375f, -1.335f), float(-PI), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(7.0f, 0.375f, -4.7f), float(-PI/2), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(5.875f, 0.375f, -6.125f), float(0), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(3.625f, 0.375f, -6.125f), float(0), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(2.2f, 0.375f, -5.0f), float(PI/2), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(2.2f, 0.375f, -2.75f), float(PI/2), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(1.345f, 0.375f, -1.55f), float(0), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(0.145f, 0.375f, -2.405f), float(-PI/2), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(0.145f, 0.375f, -4.655f), float(-PI / 2), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-0.971f, 0.375f, -6.08f), float(0), glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.221f, 0.375f, -6.08f), float(0), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.646f, 0.375f, -4.955f), float(PI/2), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.521f, 0.375f, -3.53f), float(PI), glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.321f, 0.375f, -2.675f), float(PI/2), glm::vec3(1.5f)));


	scene.push_back(Model(modelTemplates[3], glm::vec3(8.5f, -1.8f, 6.4f), float(PI/2), glm::vec3(0.05f))); //chest model

}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach

	glfwSetKeyCallback(window, Keyboard::keyCallback); // Obsługa klaiatury
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback); // Obsługa myszki
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Wyłaczenie graficznej myszki w oknie

	/*
		Tworzenie szablonów modeli
	*/
	modelTemplates.push_back(Model("wall.obj", "texture.png"));
	modelTemplates.push_back(Model("floor.obj", "stoneFloor_Albedo.png"));
	modelTemplates.push_back(Model("hole.obj", "stoneFloor_Albedo.png"));
	modelTemplates.push_back(Model("chest.obj", "chest.png"));
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();

	scene.clear(); // usuwanie modeli WIP
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	view = Camera::camera.getViewMatrix(); // wylicz nową macierz V i przekaż do modeli

	for (unsigned int i = 0; i < scene.size(); i++) { // narysuj wszystkie modele
		scene[i].render();
	}

	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}

int main()
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
		glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "sad_satan_fixed_most_final_v2.exe", glfwGetPrimaryMonitor(), NULL);
	//Utwórz okno na głównym monitorze

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące
	initModels();

	perspective = glm::perspective(glm::radians(50.0f), 1.0f, 0.5f, 50.0f); //Wylicz macierz rzutowania
	// macierz P jest stałą więc nie ma sensu jej przesyłać w pętli

	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window, deltaTime);

		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
