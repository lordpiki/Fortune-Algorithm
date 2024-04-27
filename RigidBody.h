#pragma once
#include "Vector2D.h"

class RigidBody {
public:
    float mass;
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    float r;
    float g;
    float b;

    RigidBody(float mass, const Vector2D& position, const Vector2D& velocity);

    void setRGB(float r, float g, float b);
    void applyForce(const Vector2D& force);
    void integrate(float dt);
};
