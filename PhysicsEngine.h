#pragma once
#include <vector>
#include <unordered_map>
#include "RigidBody.h"
#include "Force.h"

using std::vector;
using std::unordered_map;

class PhysicsEngine {
public:
    vector<RigidBody> setPoints;
    vector<RigidBody> corners;
    vector<Force*> forces;

    void addBody(const RigidBody& body);
    void addForce(Force* force);

    void update(float dt);
    

private:
    void calculateVoronoiRegions();

};
