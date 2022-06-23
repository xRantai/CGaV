#include "LoadedModel.h"

LoadedModel::LoadedModel() 
	: texturefile(NULL) {}

LoadedModel::LoadedModel(std::vector< glm::vec4 > x, std::vector< glm::vec2 > y, std::vector< glm::vec4 > z, std::vector<unsigned int> w, const char* r)
{
	vertices = x;
	texCoords = y;
	normals = z;
	indices = w;
	texturefile = r;
}
