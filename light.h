#ifndef LAMP_H
#define LAMP_H

#include "cube.h"

class Lamp : public Model {
public:
	glm::vec3 lightColor;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

#endif