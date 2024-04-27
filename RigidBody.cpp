#include "RigidBody.h"

RigidBody::RigidBody(float mass, const Vector2D& position, const Vector2D& velocity)
    : mass(mass), position(position), velocity(velocity), acceleration(0.0f, 0.0f), r(1.0f), g(1.0f), b(1.0f) {}

void RigidBody::setRGB(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

void RigidBody::applyForce(const Vector2D& force) {
    acceleration = force / mass;
}

void RigidBody::integrate(float dt) {
    velocity = velocity + acceleration * dt;
    position = position + velocity * dt + acceleration * 0.5f * dt * dt;
}
