#include "PhysicsEngine.h"
#include "Collision.h"
#include "Helper.h"
#include <iostream>
#include <algorithm>



void PhysicsEngine::addBody(const RigidBody& body) {
    setPoints.push_back(body);
}

void PhysicsEngine::addForce(Force* force) {
    forces.push_back(force);
}

bool PhysicsEngine::areClosestSmallerThanN(const vector<float>& distances, float n)
{
    std::vector<float> sorted_distances(distances);
    std::sort(sorted_distances.begin(), sorted_distances.end());

    // Check if there are at least 2 elements
    if (sorted_distances.size() < 2) {
        return false;
    }

    // Check if the 2 closest distances are smaller than N
    return abs(sorted_distances[0] - sorted_distances[1]) < n;
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
            float smallestDis = 10.0f;
            float disDifference = 0.01f;
            vector<float> positions;
            for (int k = 0; k < setPoints.size(); k++)
            {
                float dis = Helper::distance(setPoints[k].position, Vector2D(i, j));
                positions.push_back(dis);
            }
            if (areClosestSmallerThanN(positions, disDifference))
            {
				corners.push_back(RigidBody(10.0f, Vector2D(i, j), Vector2D(0.0f, 0.0f)));
            }

        }
    }
    std::cout << "corners size: " << corners.size() << std::endl;
    
}



