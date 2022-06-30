#include "bounds.h"

BoundingRegion::BoundingRegion() {}
BoundingRegion::BoundingRegion(glm::vec3 min, glm::vec3 max) 
	: min(min), max(max) {}

bool BoundingRegion::containsPoint(glm::vec3 pt) {
	return min.x <= pt.x && max.x >= pt.x && min.y <= pt.y && max.y >= pt.y && min.z <= pt.z && max.z >= pt.z;
}