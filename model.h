#ifndef LoadedModel_H
#define LoadedModel_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model{
public:
	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > texCoords;
	std::vector< glm::vec4 > normals;
	std::vector<unsigned int> indices;
	const char* texture;

	Model();
	Model(std::string plik, const char* texture);
};

#endif