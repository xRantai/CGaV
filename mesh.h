#ifndef MESH_H
#define MESH_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "shaderprogram.h"
#include "texture.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec2 texCoord;

	static std::vector<struct Vertex> genList(float* vertices, int noVertices);
};
typedef struct Vertex Vertex;

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<Texture> textures;

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indeces, std::vector<Texture> textures);

	void render(ShaderProgram shader);

	void cleanup();

private:
	unsigned int VBO, EBO;

	void setup();
};

#endif
