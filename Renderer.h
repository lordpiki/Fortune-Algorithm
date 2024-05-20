#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include "RigidBody.h"
#include "PhysicsEngine.h"

class Renderer {
public:
    Renderer(GLFWwindow* window);
    

    void render(const std::vector<RigidBody>& bodies, bool toClear = true);
    void render(const std::vector<Edge>& edges, bool toClear = true);

private:
    GLFWwindow* window;
};
