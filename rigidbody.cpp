#include "rigidbody.h"

RigidBody::RigidBody(glm::vec3 pos, glm::vec3 velocity, glm::vec3 acceleration) 
	: pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::update(float dt) {
	pos += velocity * dt + 0.5f * acceleration * (dt * dt);
	velocity += acceleration * dt;
}

void RigidBody::setVelocity(glm::vec3 vel, float magnitude) {
	velocity = glm::normalize(vel) * magnitude;
}