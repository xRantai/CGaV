#include "LoadedModel.h"

LoadedModel::LoadedModel(std::vector< glm::vec4 > vertices, std::vector< glm::vec2 > texCoords, std::vector< glm::vec4 > normals, std::vector<unsigned int> indices, const char* texturefile)
	: vertices(vertices), texCoords(texCoords), normals(normals), indices(indices), texturefile(texturefile) {}
