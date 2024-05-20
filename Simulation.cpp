#include "Simulation.h"
#include "GravityForce.h"
#include <iostream>
#include "FPSCounter.h"
#include "PhysicsEngine.h"
#include "Helper.h"


Simulation::Simulation(GLFWwindow* window, float fixedTimeStep)
    : window(window), renderer(window), inputManager(window), fixedTimeStep(fixedTimeStep),
    lastTime(glfwGetTime()), accumulatedTime(0.0), fpsCounter(window) {

    
 //   for (int i = 0; i < 5; i++) {
	//	RigidBody body(10.0f, Vector2D(Helper::getRand(-1, 1), Helper::getRand(-1, 1)), Vector2D(0.0f, 0.0f));
 //       body.setRGB(Helper::getRand(0, 1.0f), Helper::getRand(0, 1.0f), Helper::getRand(0, 1.0f));
	//	physicsEngine.addBody(body);
	//}

    physicsEngine.addBody(RigidBody(10.0f, Vector2D(-1.0f, -1.0f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(-0.5f, -0.5f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(0.5f, 0.5f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(1.0f, 1.0f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(-1.0f, 1.0f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(-0.5f, 0.5f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(0.5f, -0.5f), Vector2D(0.0f, 0.0f)));
    physicsEngine.addBody(RigidBody(10.0f, Vector2D(1.0f, -1.0f), Vector2D(0.0f, 0.0f)));
    RigidBody body(10.0f, Vector2D(Helper::getRand(-1, 1), Helper::getRand(-1, 1)), Vector2D(0.0f, 0.0f));
    body.setRGB(Helper::getRand(0, 1.0f), Helper::getRand(0, 1.0f), Helper::getRand(0, 1.0f));
    physicsEngine.addBody(body);
    std::cout << "Last body position: " << physicsEngine.setPoints.back().position.x << ", " << physicsEngine.setPoints.back().position.y << std::endl;





    

}

Simulation::~Simulation() {
    // Clean up resources
}

void Simulation::run() {
    while (!glfwWindowShouldClose(window)) {
        // Process input
        //inputManager.processInput(physicsEngine.bodies);

        // Calculate the elapsed time since the last frame
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - lastTime;
        lastTime = currentTime;

        fpsCounter.update();

        // Accumulate the elapsed time
        accumulatedTime += elapsedTime;

        // Update the physics simulation using the fixed time step
        while (accumulatedTime >= fixedTimeStep)
        {
            physicsEngine.update(fixedTimeStep);
            accumulatedTime -= fixedTimeStep;

        }

        // Render the scene
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.render(physicsEngine.edges, false);
        renderer.render(physicsEngine.setPoints, false);

        glfwSwapBuffers(window);


        // Render the FPS counter
        fpsCounter.render();

        // Handle other events and update the window
        glfwPollEvents();
    }
}