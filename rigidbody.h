#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>

class RigidBody {
public:
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	RigidBody(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f));

	void update(float dt);

	void setVelocity(glm::vec3 v, float magnitude);
};

#endif