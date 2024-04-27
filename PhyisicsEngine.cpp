#include "PhysicsEngine.h"
#include "Collision.h"
#include "Helper.h"
#include <iostream>

void PhysicsEngine::addBody(const RigidBody& body) {
    setPoints.push_back(body);
}

void PhysicsEngine::addForce(Force* force) {
    forces.push_back(force);
}

void PhysicsEngine::update(float dt) {
    if (corners.size() != 0) return;
    calculateVoronoiRegions();
}

void PhysicsEngine::calculateVoronoiRegions()
{
    float space = 0.01f;
    float disDifference = 0.1f;
    for (float i = -1.0f; i < 1.0f; i += space)
    {
        for (float j = -1.0f; j < 1.0f; j += space)
        {
            float smallestDis = 1000.0f;
            int smallestIndex = -1;
            for (int k = 0; k < setPoints.size(); k++)
            {
                float dis = Helper::distance(setPoints[k].position, Vector2D(i, j));
                if (dis < smallestDis)
                {
					smallestDis = dis;
					smallestIndex = k;
				}
            }
            corners.push_back(RigidBody(0.0f, Vector2D(i, j), Vector2D(0.0f, 0.0f)));
            corners[corners.size() - 1].setRGB(setPoints[smallestIndex].r, setPoints[smallestIndex].g, setPoints[smallestIndex].b);
        }
    }
    std::cout << "corners size: " << corners.size() << std::endl;
    
}



