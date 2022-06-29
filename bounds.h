#ifndef BOUNDS_H
#define BOUNDS_H

#include <glm/glm.hpp>
#include <stdlib.h>
#include <stdio.h>

enum class BoundTypes {
	AABB = 0x00,
	SPHERE = 0x01
};

class BoundingRegion {
public:
	BoundTypes type;

	// sphere values
	glm::vec3 center;
	float radius;

	// bounding box values
	glm::vec3 min;
	glm::vec3 max;

	BoundingRegion();
	BoundingRegion(BoundTypes type);
	BoundingRegion(glm::vec3 center, float radius);
	BoundingRegion(glm::vec3 min, glm::vec3 max);

	glm::vec3 calculateCenter();
	glm::vec3 calculateDimensions();

	bool containsPoint(glm::vec3 pt);
	bool intersectsWith(BoundingRegion br);
};

#endif