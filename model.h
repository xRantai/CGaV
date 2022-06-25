#ifndef LoadedModel_H
#define LoadedModel_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <rigidbody.h>
#include <bounds.h>
#include "material.h"

class Model{
public:
	BoundTypes boundType;

	glm::vec3 pos;

	RigidBody rb;
	BoundingRegion br;

	Material material;

	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > texCoords;
	std::vector< glm::vec4 > normals;
	std::vector<unsigned int> indices;
	const char* texture;

	Model();
	Model(std::string plik, const char* texture, BoundTypes boundType = BoundTypes::AABB);
};

#endif