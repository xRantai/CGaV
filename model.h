#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include "constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shaderprogram.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <textureloader.h>

#include <rigidbody.h>
#include <environment.h>
#include <bounds.h>
#include <limits>

extern glm::mat4 perspective;
extern glm::mat4 view;
extern std::vector<GLuint> textures;

class Model {
public:
	float rotation;
	glm::vec3 scale;

	unsigned int texID;

	RigidBody rb; // fizyka
	BoundingRegion br; // kolizje

	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > texCoords;
	std::vector< glm::vec4 > normals;
	std::vector<unsigned int> indices;

	Model();
	Model(std::string plik, unsigned int texID, BoundTypes boundType = BoundTypes::AABB, glm::vec3 pos = glm::vec3(0.0f), float rotation = 0.0f, glm::vec3 scale = glm::vec3(1.0f)); // tworzenie modelu z pliku
	Model(Model model, glm::vec3 pos, float rotation, glm::vec3 scale, bool hasCollission = true); // tworzenie modelu z istniejącego modelu

	void render(glm::vec3 cameraPos, glm::vec3 skullPos, float dt);

	void render2(glm::vec3 cameraPos, glm::vec3 skullPos, float dt, glm::mat4 transformation = glm::mat4(1.0f));

};

#endif