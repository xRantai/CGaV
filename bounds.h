#ifndef BOUNDS_H
#define BOUNDS_H

#include <glm/glm.hpp>
#include <stdlib.h>
#include <stdio.h>


class BoundingRegion {
public:
	// bounding box values
	glm::vec3 min;
	glm::vec3 max;

	BoundingRegion();
	BoundingRegion(glm::vec3 min, glm::vec3 max);

	bool containsPoint(glm::vec3 pt);
};

#endif