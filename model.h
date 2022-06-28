#ifndef LoadedModel_H
#define LoadedModel_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shaderprogram.h>

#include "lodepng.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <textureloader.h>

#include <rigidbody.h>
#include <environment.h>
#include <bounds.h>

#include "camera.h"

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

	Model(std::string plik, unsigned int texID, BoundTypes boundType = BoundTypes::AABB, glm::vec3 pos = glm::vec3(100000.0f), float rotation = 0.0f, glm::vec3 scale = glm::vec3(1.0f)); // tworzenie modelu z pliku
	Model(Model model, glm::vec3 pos, float rotation, glm::vec3 scale, bool hasCollission = false); // tworzenie modelu z istniej¹cego modelu

	void render(float dt);
};

#endif