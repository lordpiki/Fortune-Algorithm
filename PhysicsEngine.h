#pragma once
#include <vector>
#include <unordered_map>
#include "RigidBody.h"
#include "Force.h"

using std::vector;
using std::unordered_map;

class Edge {
public:
    Vector2D start;
    Vector2D end;
    Edge(const Vector2D& start, const Vector2D& end) : start(start), end(end) {}
};

class PhysicsEngine {
public:
    vector<RigidBody> setPoints;
    vector<Edge> edges;
    vector<Force*> forces;

    void addBody(const RigidBody& body);
    void relaxPointsNTimes(int n = 0);
    void update(float dt);
    
    static double stretch_number(double number, double min_range, double max_range, double output_min, double output_max);
    static double compress_number(double number, double min_range, double max_range, double output_min, double output_max);

private:
    void calculateVoronoiRegions();

};
