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
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
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

#include "lodepng.h"
#include "Model.h"

#include "keyboard.h"
#include "mouse.h"
#include "camera.h"

Camera Camera::camera(glm::vec3(7.0f, 1.5f, 2.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLuint tex;
GLuint tex2;
GLuint tex3;

Model wall;
Model floor_model;
Model hole;
Model chest;


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
	if (Keyboard::key(GLFW_KEY_SPACE)) {
		Camera::camera.updateCameraPos(CameraDirection::UP, dt);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		Camera::camera.updateCameraPos(CameraDirection::DOWN, dt);
	}
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
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

	tex = readTexture("texture.png");
	tex2 = readTexture("stoneFloor_Albedo.png");
	tex3 = readTexture("chest.png");

	wall = Model("wall.obj", "texture.png");
	floor_model = Model("floor.obj", "stoneFloor_Albedo.png");
	hole = Model("hole.obj", "stoneFloor_Albedo.png");
	chest = Model("chest.obj", "chest.png");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex);
	glDeleteTextures(1, &tex2);
	glDeleteTextures(1, &tex3);
}

void draw(glm::mat4 P, glm::mat4 V, glm::mat4 M, Model model, GLuint texture) {

	spTextured->use();

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, model.vertices.data()); //Współrzędne wierzchołków bierz z tablicy myCubeVertices

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, model.texCoords.data()); //Współrzędne teksturowania bierz z tablicy myCubeTexCoords

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, model.indices.data());

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}

void drawmodularwall(glm::mat4 P, glm::mat4 V, glm::mat4 M, Model model, GLuint texture, int k) {
	for (int i = 0; i < k; i++) {
		draw(P, V, M, model, texture);
		M = glm::translate(M, glm::vec3(1.5f, 0.0f, 0.0f));
	}
}

void drawmodularwall2(glm::mat4 P, glm::mat4 V, glm::mat4 M, Model model, GLuint texture, int k) {
	for (int i = 0; i < k; i++) {
		draw(P, V, M, model, texture);
		M = glm::translate(M, glm::vec3(-1.5f, 0.0f, 0.0f));
	}
}

void turnleft(glm::mat4 Mt) {
	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(1.0f, 0.0f, 0.75f));
}

void turnright(glm::mat4 Mt) {
	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(1.0f, 0.0f, 0.75f));
}

glm::mat4 drawmodularfloor(glm::mat4 P, glm::mat4 V, glm::mat4 M, Model model, GLuint texture, int k, int m) {
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < m; j++) {
			draw(P, V, M, model, texture);
			M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));
		}
		M = glm::translate(M, glm::vec3(-1.0f, 0.0f, 1.0f * m));
	}
	return M;
}

void drawfirstfloor(glm::mat4 P, glm::mat4 V) {
	glm::mat4 M = glm::mat4(1.0f); //hole drawing
	M = glm::translate(M, glm::vec3(1.1f, -0.15f, -2.7f));
	M = glm::scale(M, glm::vec3(1.2f, 1.2f, 1.2f));
	draw(P, V, M, hole, tex2);

	M = glm::mat4(1.0f); //ceiling drawing
	M = glm::translate(M, glm::vec3(8.0f, 1.8f, 6.5f));
	M = glm::scale(M, glm::vec3(2.3f, 2.3f, 2.3f));
	M = drawmodularfloor(P, V, M, floor_model, tex2, 7, 7);

	M = glm::mat4(1.0f); //floor drawing
	M = glm::translate(M, glm::vec3(8.0f, 0.0f, 6.5f));
	M = glm::scale(M, glm::vec3(2.3f, 2.3f, 2.3f));
	M = drawmodularfloor(P, V, M, floor_model, tex2, 3, 7);
	for (int j = 0; j < 4; j++) {
		draw(P, V, M, floor_model, tex2);
		M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));
	}
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));
	for (int j = 0; j < 2; j++) {
		draw(P, V, M, floor_model, tex2);
		M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));
	}
	M = glm::translate(M, glm::vec3(-1.0f, 0.0f, 1.0f * 7));
	M = drawmodularfloor(P, V, M, floor_model, tex2, 3, 7);
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));

	M = glm::mat4(1.0f); //walls drawing
	M = glm::scale(M, glm::vec3(1.5f, 1.5f, 1.5f));
	M = glm::translate(M, glm::vec3(1.0f, 0.25f, 2.0f));
	drawmodularwall(P, V, M, wall, tex, 4);
	glm::mat4 Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, wall, tex, 3);

	M = glm::rotate(M, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.57f, 0.0f, 0.80f));
	drawmodularwall(P, V, M, wall, tex, 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, M, wall, tex, 2);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, M, wall, tex, 2);


	M = glm::translate(M, glm::vec3(-3.0f, 0.0f, 0.0f));

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, M, wall, tex, 1);


	Mt = glm::mat4(M);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, wall, tex, 2);


	drawmodularwall2(P, V, M, wall, tex, 6);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -8.45f));
	drawmodularwall2(P, V, M, wall, tex, 2);


	M = glm::translate(M, glm::vec3(-3.0f, 0.0f, 0.0f));
	Mt = glm::mat4(M);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, wall, tex, 2);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 2.30f));
	drawmodularwall(P, V, Mt, wall, tex, 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.8f));
	drawmodularwall(P, V, Mt, wall, tex, 1);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-2.25f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, wall, tex, 1);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, wall, tex, 2);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, wall, tex, 2);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 2.3f));
	drawmodularwall(P, V, Mt, wall, tex, 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.8f));
	drawmodularwall2(P, V, Mt, wall, tex, 2);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, wall, tex, 2);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, wall, tex, 1);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall(P, V, Mt, wall, tex, 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.8f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	drawmodularwall2(P, V, M, wall, tex, 5);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -6.95f));
	drawmodularwall2(P, V, M, wall, tex, 7);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -9.95f));
	drawmodularwall2(P, V, M, wall, tex, 7);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -9.95f));
	drawmodularwall2(P, V, M, wall, tex, 1);
}

void drawsecondfloor(glm::mat4 P, glm::mat4 V) {
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(8.0f, -1.8f, 6.5f)); //floor drawing
	M = glm::scale(M, glm::vec3(2.3f, 2.3f, 2.3f));
	M = drawmodularfloor(P, V, M, floor_model, tex2, 7, 7);

	M = glm::mat4(1.0f); //walls drawing
	M = glm::translate(M, glm::vec3(-5.5f, -1.45f, 0.75f));
	M = glm::scale(M, glm::vec3(1.5f, 1.5f, 1.5f));
	M = glm::translate(M, glm::vec3(9.10f, 0.0f, -1.55f));
	drawmodularwall2(P, V, M, wall, tex, 2);

	glm::mat4 Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.80f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, -0.80f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, M, wall, tex, 3);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 3.57f));
	drawmodularwall(P, V, M, wall, tex, 7);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 9.57f));
	drawmodularwall(P, V, M, wall, tex, 7);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 3.57f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, wall, tex, 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, wall, tex, 2);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 5.07f));
	drawmodularwall(P, V, Mt, wall, tex, 2);

	glm::mat4 Mt2 = glm::mat4(Mt);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, wall, tex, 2);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, wall, tex, 2);

	glm::mat4 Mt3 = glm::mat4(Mt);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt3 = glm::rotate(Mt3, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt3 = glm::translate(Mt3, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt3, wall, tex, 2);

	Mt3 = glm::rotate(Mt3, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt3 = glm::translate(Mt3, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt3, wall, tex, 1);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -2.25f));
	drawmodularwall(P, V, Mt2, wall, tex, 1);

	Mt2 = glm::rotate(Mt2, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt2, wall, tex, 1);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt2, wall, tex, 1);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt2, wall, tex, 2);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -2.25f));
	drawmodularwall(P, V, Mt2, wall, tex, 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 9.57f));
	drawmodularwall(P, V, M, wall, tex, 7);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 5.07f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 9.57f));
	drawmodularwall(P, V, M, wall, tex, 4);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, wall, tex, 1);

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(8.5f, -1.8f, 6.4f));
	M = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	draw(P, V, M, chest, tex3);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	glm::mat4 V = Camera::camera.getViewMatrix();
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 0.5f, 50.0f); //Wylicz macierz rzutowania

	drawfirstfloor(P, V);
	drawsecondfloor(P, V);


	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "sad_satan_fixed_most_final_v2.exe", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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
	
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		double currentTime = glfwGetTime();
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
