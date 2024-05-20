#include "Renderer.h"
#include <iostream>

using std::cout;
using std::endl;

Renderer::Renderer(GLFWwindow* window) : window(window) {}

void Renderer::render(const std::vector<RigidBody>& bodies, bool toClear)
{
    if (toClear)
    {
		glClear(GL_COLOR_BUFFER_BIT);

	}

    for (const RigidBody& body : bodies)
    {
        //draw a circle
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(body.r, body.g, body.b);
        glVertex2f(body.position.x, body.position.y);
        for (int i = 0; i <= 300; i++)
        {
			double angle = 2.0 * 3.1415926 * double(i) / 300;
			double x = 0.01f * cosf(angle);
			double y = 0.01f * sinf(angle);

			glVertex2f(x + body.position.x, y + body.position.y);
		}
        glEnd();
	}
}



void Renderer::render(const std::vector<Edge>& edges, bool toClear) {

    if (toClear)
    {
        glClear(GL_COLOR_BUFFER_BIT);

    }

    for (const Edge& edge: edges)
    {
            
		glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(edge.start.x, edge.start.y);
        glVertex2f(edge.end.x, edge.end.y);

        glEnd();

    }
    //std::cout << "Edge size: " << edges.size() << std::endl;

}
