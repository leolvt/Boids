#include <iostream>

#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

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
const int TICKS_PER_SECOND = 10;
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

/* Th flock of boids*/
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

    // Create the flock
    flock = new Flock(100);

    glm::vec3 V1(1,0,-1);
    glm::vec3 V2(0,1,1);
    std::cout << "Angle: " << glm::angle(glm::normalize(V1), glm::normalize(V2)) << std::endl;

}

// ============================================= //

void keyPressed(int key, int status)
{
    if(status == GLFW_RELEASE) return;
    if(key == GLFW_KEY_LEFT)
    {
        eyePhi -= DELTA_ANGLE;
        if(eyePhi < 0) eyePhi += 2 * Util::PI;
        if(eyePhi > 2 * Util::PI) eyePhi -= 2 * Util::PI;
    }
    else if(key == GLFW_KEY_RIGHT)
    {
        eyePhi += DELTA_ANGLE;
        if(eyePhi < 0) eyePhi += 2 * Util::PI;
        if(eyePhi > 2 * Util::PI) eyePhi -= 2 * Util::PI;
    }
    else if(key == GLFW_KEY_UP)
    {
        eyeTheta += DELTA_ANGLE;
        if(eyeTheta < -Util::PI / 2) eyeTheta = -Util::PI / 2;
        if(eyeTheta > Util::PI / 2) eyeTheta = Util::PI / 2;
    }
    else if(key == GLFW_KEY_DOWN)
    {
        eyeTheta -= DELTA_ANGLE;
        if(eyeTheta < -Util::PI / 2) eyeTheta = -Util::PI / 2;
        if(eyeTheta > Util::PI / 2) eyeTheta = Util::PI / 2;
    }

    // Compute OpenGL Coordinates (Cartesian)
    eyeX = eyeRadius * cos(eyeTheta) * sin(eyePhi);
    eyeY = eyeRadius * sin(eyeTheta);
    eyeZ = eyeRadius * cos(eyeTheta) * cos(eyePhi);

    upX = eyeRadius * cos(eyeTheta + 0.1) * sin(eyePhi) - eyeX;
    upY = eyeRadius * sin(eyeTheta + 0.1) - eyeY;
    upZ = eyeRadius * cos(eyeTheta + 0.1) * cos(eyePhi) - eyeZ;

    std::cout << "Eye Polar (T, P): " << eyeTheta << ", " << eyePhi << std::endl;
    std::cout << "Eye at: " << eyeX << ", " << eyeY << ", " << eyeZ << std::endl << std::endl;
}

// ============================================= //

void update()
{
    if (flock != 0) flock->update();
}

// ============================================= //

void draw()
{
    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ,   0, 0, 0,   upX, upY, upZ);

    // Set Perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.33, 1.6, 100);
    glMatrixMode(GL_MODELVIEW);

    if (flock != 0)
    {
        flock->draw();
    }

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
