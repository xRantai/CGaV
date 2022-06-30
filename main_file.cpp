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

#include <textureloader.h>
#include "model.h"

#include "keyboard.h"
#include "mouse.h"
#include "camera.h"

Camera Camera::camera(glm::vec3(7.0f, 1.0f, 2.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Model torch; 
Model skull;

TextureLoader textureLoader;
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
	if (Keyboard::keyWentDown(GLFW_KEY_SPACE)) {
		Camera::camera.updateCameraPos(CameraDirection::UP, dt);
	}
}

void initModels() {
	skull = Model(modelTemplates[5], glm::vec3(7.0f, 1.5f, 1.0f), 0.0f, glm::vec3(0.3f), false); // skull

	for (int i = 0; i < 7; i++) { //ceiling
		for (int j = 0; j < 7; j++) {
			scene.push_back(Model(modelTemplates[1], glm::vec3(8.0f - 2.3f * i, 1.8f, 6.5f - 2.3f * j), Angles::d0, glm::vec3(2.3f)));
			scene[scene.size() - 1].br.max = glm::vec3(100.0f, 100.0f, 100.0f);
			scene[scene.size() - 1].br.min = glm::vec3(-100.0f, 1.8f, -100.0f);
		}
	}
	for (int i = 0; i < 7; i++) { //first floor floor
		for (int j = 0; j < 7; j++) {
			if (i == 3 && j == 4) { continue; }
			scene.push_back(Model(modelTemplates[1], glm::vec3(8.0f - 2.3f * i, 0.0f, 6.5f - 2.3f * j), Angles::d0, glm::vec3(2.3f)));
			scene[scene.size() - 1].br = BoundingRegion();
		}
	}
	scene[scene.size() - 1].br.max = glm::vec3(0.5f, 0.1f, 100.0f);
	scene[scene.size() - 1].br.min = glm::vec3(-100.0f, -0.1f, -100.0f);

	scene[scene.size() - 2].br.max = glm::vec3(100.0f, 0.1f, 100.0f);
	scene[scene.size() - 2].br.min = glm::vec3(1.9f, -0.1f, -100.0f);

	scene[scene.size() - 3].br.max = glm::vec3(100.0f, 0.1f, 100.0f);
	scene[scene.size() - 3].br.min = glm::vec3(-100.0f, -0.1f, -1.6f);

	scene[scene.size() - 4].br.max = glm::vec3(100.0f, 0.1f, -3.6f);
	scene[scene.size() - 4].br.min = glm::vec3(-100.0f, -0.1f, -100.0f);


	for (int i = 0; i < 7; i++) { //second floor floor
		for (int j = 0; j < 7; j++) {
			scene.push_back(Model(modelTemplates[1], glm::vec3(8.0f - 2.3f * i, -1.8f, 6.5f - 2.3f * j), Angles::d0, glm::vec3(2.3f)));
			scene[scene.size() - 1].br.max = glm::vec3(100.0f, -1.8f, 100.0f);
			scene[scene.size() - 1].br.min = glm::vec3(-100.0f, -100.0f, -100.0f);
		}
	}

	scene.push_back(Model(modelTemplates[2], glm::vec3(1.1f, -0.15f, -2.7f), Angles::d0, glm::vec3(1.2f), false)); //hole model

	for (int i = 0; i < 4; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(8.25f - i * 2.25f, 0.375f, 3.0f), Angles::d0, glm::vec3(1.5f))); //first floor walls
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(0.075f, 0.375f, 4.125f), Angles::d90, glm::vec3(1.5f)));

	for (int i = 0; i < 3; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(1.2f + i * 2.25f, 0.375f, 5.55f), Angles::d180, glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(0.3f, 0.375f, 2.145f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-0.825f, 0.375f, 0.72f), Angles::d0, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.075f, 0.375f, 0.72f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.5f, 0.375f, 1.845f), Angles::d90, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.5f, 0.375f, 4.095f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.25f, 0.375f, 4.095f), Angles::d90, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.25f, 0.375f, 6.345f), Angles::d90, glm::vec3(1.5f)));

	for (int i = 0; i < 5; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-1.125f + 2.25f * i, 0.375f, 7.77f), Angles::d180, glm::vec3(1.5f)));
	}

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(9.3f, 0.375f, 6.645f - 2.25f * i), Angles::d270, glm::vec3(1.5f)));
	}

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(8.175f - 2.25f * i, 0.375f, -8.28f), Angles::d0, glm::vec3(1.5f)));
	}

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-6.75f, 0.375f, -7.155f + 2.25f * i), Angles::d90, glm::vec3(1.5f)));
	}

	for (int i = 0; i < 2; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-5.625f + 2.25f * i, 0.375f, 7.77f), Angles::d180, glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(8.175f, 0.375f, 0.72f), Angles::d0, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(5.925f, 0.375f, 0.72f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(4.725f, 0.375f, -0.135f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(5.575f, 0.375f, -1.335f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(7.0f, 0.375f, -4.7f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(5.875f, 0.375f, -6.125f), Angles::d0, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(3.625f, 0.375f, -6.125f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(2.2f, 0.375f, -5.0f), Angles::d90, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(2.2f, 0.375f, -2.75f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(1.345f, 0.375f, -1.55f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(0.145f, 0.375f, -2.405f), Angles::d270, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(0.145f, 0.375f, -4.655f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-0.971f, 0.375f, -6.08f), Angles::d0, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.221f, 0.375f, -6.08f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.646f, 0.375f, -4.955f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.521f, 0.375f, -3.53f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.321f, 0.375f, -2.675f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(8.15f, -1.45f, -1.575f), Angles::d0, glm::vec3(1.5f))); //second floor walls
	scene.push_back(Model(modelTemplates[0], glm::vec3(5.9f, -1.45f, -1.575f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(4.475f, -1.45f, -0.45f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(3.62f, -1.45f, 0.75f), Angles::d0, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(1.32f, -1.45f, 0.75f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(2.195f, -1.45f, 1.875f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(8.15f, -1.45f, -1.575f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(9.3f, -1.45f, -2.25f), Angles::d270, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(9.3f, -1.45f, -4.55f), Angles::d270, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(9.3f, -1.45f, -6.85f), Angles::d270, glm::vec3(1.5f)));

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(8.175f - i*2.3f, -1.45f, -8.275f), Angles::d0, glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(4.7f, -1.45f, -6.85f), Angles::d270, glm::vec3(1.5f)));

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-7.05f, -1.45f, -7.15f + i*2.3f), Angles::d90, glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(-5.925f, -1.45f, -5.725f), Angles::d180, glm::vec3(1.5f)));
	
	scene.push_back(Model(modelTemplates[0], glm::vec3(-5.925f, -1.45f, -1.125f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.495f, -1.45f, -2.25f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.64f, -1.45f, -3.45f), Angles::d180, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-1.34f, -1.45f, -3.45f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.215f, -1.45f, -4.575f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-5.925f, -1.45f, 1.175f), Angles::d180, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.62f, -1.45f, 1.175f), Angles::d180, glm::vec3(1.5f)));//tu

	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.195f, -1.45f, 0.05f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-1.34f, -1.45f, -1.15f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(0.96f, -1.45f, -1.15f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(2.385f, -1.45f, -2.275f), Angles::d270, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(2.385f, -1.45f, -4.575f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(3.24f, -1.45f, -3.475f), Angles::d180, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(5.54f, -1.45f, -3.475f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(6.965f, -1.45f, -4.6f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-2.42f, -1.45f, 2.03f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-1.295f, -1.45f, 3.455f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-0.095f, -1.45f, 4.31f), Angles::d90, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-0.95f, -1.45f, 5.51f), Angles::d0, glm::vec3(1.5f)));
	scene.push_back(Model(modelTemplates[0], glm::vec3(-3.25f, -1.45f, 5.51f), Angles::d0, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(-4.45f, -1.45f, 4.655f), Angles::d270, glm::vec3(1.5f)));

	for (int i = 0; i < 7; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(-5.925f + i * 2.3f, -1.45f, 8.075f), Angles::d180, glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(2.4f, -1.45f, 6.95f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(3.255f, -1.45f, 5.75f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(4.68f, -1.45f, 4.625f), Angles::d270, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(5.535f, -1.45f, 3.425f), Angles::d180, glm::vec3(1.5f)));

	scene.push_back(Model(modelTemplates[0], glm::vec3(6.96f, -1.45f, 2.3f), Angles::d270, glm::vec3(1.5f)));

	for (int i = 0; i < 4; i++) {
		scene.push_back(Model(modelTemplates[0], glm::vec3(9.303f, -1.45f, 6.95f-2.3f*i), Angles::d270, glm::vec3(1.5f)));
	}

	scene.push_back(Model(modelTemplates[0], glm::vec3(8.178f, -1.45f, 5.525f), Angles::d0, glm::vec3(1.5f)));


	scene.push_back(Model(modelTemplates[3], glm::vec3(8.5f, -1.8f, 6.85f), Angles::d90, glm::vec3(0.05f))); //chest model

	torch = Model(modelTemplates[4], glm::vec3(0.3f, -0.5f, -1.0f), -float(Camera::camera.yaw * PI / 180 + PI / 2), glm::vec3(0.5f), false);
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

	textures.push_back(textureLoader.load("texture.png"));
	modelTemplates.push_back(Model("wall.obj", textureLoader.getCurrentID()));

	textures.push_back(textureLoader.load("stoneFloor_Albedo.png"));
	modelTemplates.push_back(Model("floor.obj", textureLoader.getCurrentID()));
	modelTemplates.push_back(Model("hole.obj", textureLoader.getCurrentID()));

	textures.push_back(textureLoader.load("chest.png"));
	modelTemplates.push_back(Model("chest.obj", textureLoader.getCurrentID()));

	textures.push_back(textureLoader.load("torch.png"));
	modelTemplates.push_back(Model("torch.obj", textureLoader.getCurrentID()));

	textures.push_back(textureLoader.load("wall_spec.png"));
	textures.push_back(textureLoader.load("floor_spec.png"));
	textures.push_back(textureLoader.load("chest_spec.png"));
	textures.push_back(textureLoader.load("torch.png"));

	textures.push_back(textureLoader.load("skull.png"));
	modelTemplates.push_back(Model("skull.obj", textureLoader.getCurrentID(), BoundTypes::SPHERE));
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();

	textureLoader.destroyTextures();
	scene.clear(); // usuwanie modeli

}

float updateSkull() {
	glm::vec3 direction = skull.rb.pos - Camera::camera.rb.pos;
	skull.rb.setVelocity(direction, -1.0f); // skull follows you
	skull.rotation = glm::atan(direction.x, direction.z) - PI; // looks your direction
	float rotation = glm::atan(direction.y, direction.x);
	return rotation;
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float dt) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	view = Camera::camera.getViewMatrix(); // wylicz nową macierz V i przekaż do modeli

	
	glm::mat4 M = glm::mat4(1.0f);
	torch.rotation = -float(Camera::camera.yaw * PI / 180 + PI / 2);
	M = glm::translate(M, Camera::camera.rb.pos);

	torch.render2(Camera::camera.rb.pos, skull.rb.pos, dt, M);
	float rotation = updateSkull();
	skull.render3(Camera::camera.rb.pos, skull.rb.pos, dt, rotation);


	bool ground = false, ceiling = false, wallX = false, wallZ = false;
	RigidBody tempX = RigidBody(Camera::camera.rb.mass, Camera::camera.rb.pos, glm::vec3(Camera::camera.rb.velocity.x, 0.0f, 0.0f), Camera::camera.rb.acceleration);
	tempX.update(dt);
	RigidBody tempYGround = RigidBody(Camera::camera.rb.mass, Camera::camera.rb.pos - glm::vec3(0.0f, 0.6f, 0.0f), glm::vec3(0.0f, Camera::camera.rb.velocity.y, 0.0f), Camera::camera.rb.acceleration);
	tempYGround.update(dt);
	RigidBody tempYCeiling = RigidBody(Camera::camera.rb.mass, Camera::camera.rb.pos + glm::vec3(0.0f, 0.1f, 0.0f), glm::vec3(0.0f, Camera::camera.rb.velocity.y, 0.0f), Camera::camera.rb.acceleration);
	tempYCeiling.update(dt);
	RigidBody tempZ = RigidBody(Camera::camera.rb.mass, Camera::camera.rb.pos, glm::vec3(0.0f, 0.0f, Camera::camera.rb.velocity.z), Camera::camera.rb.acceleration);
	tempZ.update(dt);

	//printf("%f %f %f\n", Camera::camera.rb.pos.x, Camera::camera.rb.pos.y, Camera::camera.rb.pos.z);

	for (Model &object : scene) { // narysuj wszystkie modele
		object.render(Camera::camera.rb.pos, scene[0].rb.pos, dt);

		if (object.br.containsPoint(tempX.pos)) {
			wallX = true;
		}
		if (object.br.containsPoint(tempYGround.pos)) {
			ground = true;
		}
		if (object.br.containsPoint(tempZ.pos)) {
			wallZ = true;
		}
		if (object.br.containsPoint(tempYCeiling.pos)) {
			ceiling = true;
		}
	}

	RigidBody out;

	if (!wallX) { // przesunięcie w osi X
		Camera::camera.rb.pos.x = tempX.pos.x;
	}

	if (ground) { // Jeżeli na ziemi step assist
		Camera::camera.rb.velocity.y = 0.01f;
	}
	else if (ceiling) { // Jeżeli głowa w suficie spadaj
		Camera::camera.rb.velocity.y = -0.01f;
	}
	else { //dryfujesz sobie gdzieś nie wnikam
		Camera::camera.rb.pos.y = tempYGround.pos.y + 0.6f;
		Camera::camera.rb.velocity.y = tempYGround.velocity.y;
	}

	if (!wallZ) { // przesunięcie w osi Z
		Camera::camera.rb.pos.z = tempZ.pos.z;
	}
	
	Camera::camera.rb.velocity = glm::vec3(0, Camera::camera.rb.velocity.y, 0);

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

	//window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "sad_satan_fixed_most_final_v2.exe", glfwGetPrimaryMonitor(), NULL);
	//Okno na pokaz ^ Okno debug v
	window = glfwCreateWindow(1000, 1000, "sad_satan_fixed_most_final_v2.exe", NULL, NULL);

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
	//scene.push_back(Model(modelTemplates[3], glm::vec3(0.0f), Angles::d270, glm::vec3(0.5f)));

	perspective = glm::perspective(glm::radians(50.0f), 1.0f, 0.5f, 50.0f); //Wylicz macierz rzutowania
	// macierz P jest stałą więc nie ma sensu jej przesyłać w pętli
	
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		processInput(window, deltaTime);

		drawScene(window, deltaTime); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
