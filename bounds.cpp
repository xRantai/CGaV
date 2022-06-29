#include "bounds.h"

BoundingRegion::BoundingRegion() {}
BoundingRegion::BoundingRegion(BoundTypes type) 
	: type(type) {}
BoundingRegion::BoundingRegion(glm::vec3 center, float radius)
	: type(BoundTypes::SPHERE), center(center), radius(radius) {}
BoundingRegion::BoundingRegion(glm::vec3 min, glm::vec3 max)
	: type(BoundTypes::AABB), min(min), max(max) {}

glm::vec3 BoundingRegion::calculateCenter(){
	return (type == BoundTypes::AABB) ? (min + max) / 2.0f : center;
}
glm::vec3 BoundingRegion::calculateDimensions(){
	return (type == BoundTypes::AABB) ? (max - min) : glm::vec3(2.0f * radius);
}

bool BoundingRegion::containsPoint(glm::vec3 pt){
	return (pt.x >= min.x) && (pt.x <= max.x) && (pt.y >= min.y) && (pt.y <= max.y) && (pt.z >= min.z) && (pt.z <= min.z);
}
bool BoundingRegion::intersectsWith(BoundingRegion br){
	if (type == BoundTypes::AABB and br.type == BoundTypes::AABB) {
		glm::vec3 rad = calculateDimensions();
		glm::vec3 radBr = br.calculateDimensions();

		glm::vec3 center = calculateCenter();
		glm::vec3 centerBr = br.calculateCenter();

		glm::vec3 dist = abs(center - centerBr);

		for (int i = 0; i < 3; i++) {
			if (dist[i] > rad[i] + radBr[i]) {
				return false;
			}
		}

		return true;
	}
	else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE) {
		return glm::length(center - br.center) < (radius + br.radius);
	}
	else if (type == BoundTypes::SPHERE) {
		float distSquared = 0.0f;
		for (int i = 0; i < 3; i++) {
			if (center[i] < br.min[i]) {
				// beyond min
				distSquared += (br.min[i] - center[i]) * (br.min[i] - center[i]);
			}
			else if (center[i] > br.max[i]) {
				// beyond max
				distSquared += (center[i] - br.max[i]) * (center[i] - br.max[i]);
			}
			// else inside
		}

		return distSquared < (radius* radius);
	}
	else {
		return br.intersectsWith(*this);
	}

}