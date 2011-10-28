#include <iostream>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Boid.h"

Boids::Boid* myBoid = 0;
Boids::Boid* myBoid2 = 0;

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 2.5, 5,   0, 0, 0,   0, 1, 0);

    // Set Perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.33, 1.6, 100);
    glMatrixMode(GL_MODELVIEW);

    if (myBoid != 0)
    {
        myBoid->update();
        myBoid->draw();

        myBoid2->update();
        myBoid2->draw();
    }

    glfwSwapBuffers();
}

int main()
{
    std::cout << "Starting GLFW and opening window." << std::endl;
    glfwInit();
    glfwOpenWindow(800, 600, 8, 8, 8, 8, 8, 0, GLFW_WINDOW);
    glEnable(GL_DEPTH_TEST);
    std::cout << "Starting GLEW." << std::endl;

    std::cout << "Creating Boid" << std::endl;
    glm::vec3 orig(-2,0,0);
    myBoid = new Boids::Boid(orig, true);
    orig = glm::vec3(-2,0,-2);
    myBoid2 = new Boids::Boid(orig, true);

    std::cout << "Starting Main Loop." << std::endl;
    glClearColor(1.0, 1.0, 1.0, 1.0);
    while(true)
    {
        draw();
        if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS || !glfwGetWindowParam(GLFW_OPENED))
            break;
    }

    std::cout << "Starting Ending." << std::endl;
    glfwTerminate();

    if (myBoid != 0) delete myBoid;
    if (myBoid2 != 0) delete myBoid2;

    return 0;
}
