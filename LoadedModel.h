#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#ifndef LoadedModel_H
#define LoadedModel_H

class LoadedModel
{
public:
	std::vector< glm::vec4 > vertices;
	std::vector< glm::vec2 > texCoords;
	std::vector< glm::vec4 > normals;
	std::vector<unsigned int> indices;
	const char* texturefile;

	LoadedModel(std::vector< glm::vec4 >, std::vector< glm::vec2 >, std::vector< glm::vec4 >, std::vector<unsigned int>, const char* r);
};

#endif