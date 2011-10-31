#include <iostream>

#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Util.h"
#include "Boid.h"
#include "Flock.h"
#include "World.h"

namespace Boids {
namespace World {

// ============================================= //

/* Annonymous Namespace to contain "private" data */
namespace {

/* Define some variables to control FPS and Game Update Rate */
const int TICKS_PER_SECOND = 50;
const float SKIP_TICKS = 1.0 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 10;

/* Camera related variables */
const float DELTA_ANGLE = Util::PI / 36;
float eyeX = 0;
float eyeY = 0;
float eyeZ = 10;
float upX = 0;
float upY = 1;
float upZ = 0;
float eyeRadius = 10;
float eyeTheta = 0.0;
float eyePhi = 0.0;
glm::vec3 center(0,0,0);

// The leader
Boid* leader = 0;

// The flock of boids
Flock* flock = 0;

};

// ============================================= //

void initialize()
{
    // Create Window and set callbacks
    std::cout << "Starting GLFW and opening window." << std::endl;
    glfwInit();
    glfwOpenWindow(800, 600, 8, 8, 8, 8, 8, 0, GLFW_WINDOW);
    glfwSetWindowTitle("Boids - Bird-like flocking behaviour simulation");
    glfwSetKeyCallback(keyPressed);
    glEnable(GL_DEPTH_TEST);

    // Inform the boids about the FPS
    Boid::setFPS(TICKS_PER_SECOND);

    // Create the Leader
    leader = new Boid(glm::vec3(0,0,0));
    leader->setVelocity(glm::vec3(0,0,-1));

    // Create the flock
    flock = new Flock(100);

    std::cout << "sin(30) = " << glm::sin(30.0) << std::endl;

    glm::vec3 car(1,1,1);
    glm::vec3 sphe = Util::cartesianToSpherical(car);
    std::cout << "R:" << sphe.x << std::endl;
    std::cout << "theta: " << sphe.y << std::endl;
    std::cout << "phi: " << sphe.z << std::endl;

    sphe.z = (-135)*Util::PI/180;
    std::cout << "New Phi: " << sphe.z << std::endl;

    car = Util::sphericalToCartesian(sphe);
    std::cout << "X:" << car.x << std::endl;
    std::cout << "Y: " << car.y << std::endl;
    std::cout << "Z: " << car.z << std::endl;

    //exit(0);
}

// ============================================= //

void keyPressed(int key, int status)
{
    if(status == GLFW_RELEASE) return;

    double angleInc = 15.0;
    if(key == GLFW_KEY_LEFT)
    {
        leader->rotateYaw(-angleInc);
    }
    else if(key == GLFW_KEY_RIGHT)
    {
        leader->rotateYaw(+angleInc);
    }
    else if(key == GLFW_KEY_UP)
    {
        leader->rotatePitch(angleInc);
    }
    else if(key == GLFW_KEY_DOWN)
    {
        leader->rotatePitch(-angleInc);
    }

}

// ============================================= //

void update()
{
    // Update the leader
    if (leader != 0) leader->update();

    // Update the flock
    //if (flock != 0) flock->update( leader->getPosition() );
    if (flock != 0) flock->update( glm::vec3(0,-5,-10) );
}

// ============================================= //

void draw()
{
    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ,   center.x, center.y, center.z,   upX, upY, upZ);

    // Set Perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.33, 1.6, 100);
    glMatrixMode(GL_MODELVIEW);

    // Draw the leader
    if (leader != 0) leader->draw();

    // Draw the flock
    if (flock != 0) flock->draw();

    glfwSwapBuffers();
}

// ============================================= //

void run()
{
    std::cout << "Starting Main Loop." << std::endl;
    glClearColor(1.0, 1.0, 1.0, 1.0);

    double nextGameTick = glfwGetTime();
    int loops;
    bool running = true;
    while( running ) {

        /* Update the simulation at rate defined in TICS_PER_SECOND ,
         * Allowing a max frameskip rate defined at MAX_FRAMESKIP
         */
        loops = 0;
        while( glfwGetTime() > nextGameTick && loops < MAX_FRAMESKIP) {
            update();
            nextGameTick += SKIP_TICKS;
            loops++;
        }

        // Draws the scene, as fast as possible
        draw();

        // Check for ESC key or windows Close
        if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS ||
            !glfwGetWindowParam(GLFW_OPENED))
        {
            running = false;
        }
    }
}

// ============================================= //

void terminate()
{
    if (flock != 0)
    {
        delete flock;
        flock = 0;
    }

    /* No more GLFW... */
    std::cout << "Terminating GLFW." << std::endl;
    glfwTerminate();
}

// ============================================= //

};
};
