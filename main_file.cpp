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
std::vector<Model> models; // wszystkie renderowane modele


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
	models.push_back(Model(modelTemplates[1], glm::vec3(1.0f), float(3.1459 / 2), glm::vec3(1.0f)));
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

	models.clear(); // usuwanie modeli WIP
}

/*void draw(glm::mat4 P, glm::mat4 V, glm::mat4 M, Model model, GLuint texture) {

	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu


	glEnableVertexAttribArray(shader->a("aPos"));
	glVertexAttribPointer(shader->a("aPos"), 4, GL_FLOAT, false, 0, model.vertices.data());

	glEnableVertexAttribArray(shader->a("aNormal"));
	glVertexAttribPointer(shader->a("aNormal"), 4, GL_FLOAT, false, 0, model.normals.data());

	glEnableVertexAttribArray(shader->a("aTexCoord"));
	glVertexAttribPointer(shader->a("aTexCoord"), 2, GL_FLOAT, false, 0, model.texCoords.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(shader->u("tex"), 0);

	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, model.indices.data());

	glDisableVertexAttribArray(shader->a("aPos"));
	glDisableVertexAttribArray(shader->a("aNormal"));
	glDisableVertexAttribArray(shader->a("aTexCoord"));
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
	glm::mat4 M = glm::mat4(1.0f); //model[2] drawing
	M = glm::translate(M, glm::vec3(1.1f, -0.15f, -2.7f));
	M = glm::scale(M, glm::vec3(1.2f, 1.2f, 1.2f));
	draw(P, V, M, model[2], tex[1]);

	M = glm::mat4(1.0f); //ceiling drawing
	M = glm::translate(M, glm::vec3(8.0f, 1.8f, 6.5f));
	M = glm::scale(M, glm::vec3(2.3f, 2.3f, 2.3f));
	M = drawmodularfloor(P, V, M, model[1], tex[1], 7, 7);

	M = glm::mat4(1.0f); //model[1] drawing
	M = glm::translate(M, glm::vec3(8.0f, 0.0f, 6.5f));
	M = glm::scale(M, glm::vec3(2.3f, 2.3f, 2.3f));
	M = drawmodularfloor(P, V, M, model[1], tex[1], 3, 7);
	for (int j = 0; j < 4; j++) {
		draw(P, V, M, model[1], tex[1]);
		M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));
	}
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));
	for (int j = 0; j < 2; j++) {
		draw(P, V, M, model[1], tex[1]);
		M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));
	}
	M = glm::translate(M, glm::vec3(-1.0f, 0.0f, 1.0f * 7));
	M = drawmodularfloor(P, V, M, model[1], tex[1], 3, 7);
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, -1.0f));

	M = glm::mat4(1.0f); //walls drawing
	M = glm::scale(M, glm::vec3(1.5f, 1.5f, 1.5f));
	M = glm::translate(M, glm::vec3(1.0f, 0.25f, 2.0f));
	drawmodularwall(P, V, M, model[0], tex[0], 4);
	glm::mat4 Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 3);

	M = glm::rotate(M, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.57f, 0.0f, 0.80f));
	drawmodularwall(P, V, M, model[0], tex[0], 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, M, model[0], tex[0], 2);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, M, model[0], tex[0], 2);


	M = glm::translate(M, glm::vec3(-3.0f, 0.0f, 0.0f));

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, M, model[0], tex[0], 1);


	Mt = glm::mat4(M);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 2);


	drawmodularwall2(P, V, M, model[0], tex[0], 6);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -8.45f));
	drawmodularwall2(P, V, M, model[0], tex[0], 2);


	M = glm::translate(M, glm::vec3(-3.0f, 0.0f, 0.0f));
	Mt = glm::mat4(M);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 2);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 2.30f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.8f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-2.25f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 1);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 2);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 2);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 2.3f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.8f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 2);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 2);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 1);


	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.8f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	drawmodularwall2(P, V, M, model[0], tex[0], 5);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -6.95f));
	drawmodularwall2(P, V, M, model[0], tex[0], 7);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -9.95f));
	drawmodularwall2(P, V, M, model[0], tex[0], 7);


	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-0.75f, 0.0f, -9.95f));
	drawmodularwall2(P, V, M, model[0], tex[0], 1);
}

void drawsecondfloor(glm::mat4 P, glm::mat4 V) {
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(8.0f, -1.8f, 6.5f)); //model[1] drawing
	M = glm::scale(M, glm::vec3(2.3f, 2.3f, 2.3f));
	M = drawmodularfloor(P, V, M, model[1], tex[1], 7, 7);

	M = glm::mat4(1.0f); //walls drawing
	M = glm::translate(M, glm::vec3(-5.5f, -1.45f, 0.75f));
	M = glm::scale(M, glm::vec3(1.5f, 1.5f, 1.5f));
	M = glm::translate(M, glm::vec3(9.10f, 0.0f, -1.55f));
	drawmodularwall2(P, V, M, model[0], tex[0], 2);

	glm::mat4 Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -2.45f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, 0.80f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.75f, 0.0f, -0.95f));
	drawmodularwall2(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(-0.57f, 0.0f, -0.80f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, M, model[0], tex[0], 3);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 3.57f));
	drawmodularwall(P, V, M, model[0], tex[0], 7);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 9.57f));
	drawmodularwall(P, V, M, model[0], tex[0], 7);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 3.57f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);


	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 2);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 5.07f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 2);

	glm::mat4 Mt2 = glm::mat4(Mt);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 2);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 2);

	glm::mat4 Mt3 = glm::mat4(Mt);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt3 = glm::rotate(Mt3, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt3 = glm::translate(Mt3, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt3, model[0], tex[0], 2);

	Mt3 = glm::rotate(Mt3, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt3 = glm::translate(Mt3, glm::vec3(0.8f, 0.0f, 2.07f));
	drawmodularwall(P, V, Mt3, model[0], tex[0], 1);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -2.25f));
	drawmodularwall(P, V, Mt2, model[0], tex[0], 1);

	Mt2 = glm::rotate(Mt2, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt2, model[0], tex[0], 1);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt2, model[0], tex[0], 1);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt2, model[0], tex[0], 2);

	Mt2 = glm::rotate(Mt2, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt2 = glm::translate(Mt2, glm::vec3(0.95f, 0.0f, -2.25f));
	drawmodularwall(P, V, Mt2, model[0], tex[0], 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 9.57f));
	drawmodularwall(P, V, M, model[0], tex[0], 7);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 5.07f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, -3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.95f, 0.0f, -0.75f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(0.8f, 0.0f, 9.57f));
	drawmodularwall(P, V, M, model[0], tex[0], 4);

	Mt = glm::mat4(M);
	Mt = glm::rotate(Mt, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	Mt = glm::translate(Mt, glm::vec3(0.8f, 0.0f, 0.57f));
	drawmodularwall(P, V, Mt, model[0], tex[0], 1);

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(8.5f, -1.8f, 6.4f));
	M = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
	M = glm::rotate(M, 3.14159f / 2, glm::vec3(0.0f, 1.0f, 0.0f));
	draw(P, V, M, model[3], tex[2]);
}*/

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	view = Camera::camera.getViewMatrix();


	for (unsigned int i = 0; i < models.size(); i++) {
		//models[i].update(); // zaktualizuj models (WIP)
		models[i].render();
	}

	/*drawfirstfloor(perspective, view);
	drawsecondfloor(perspective, view);*/


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
