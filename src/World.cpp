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

// Define some variables to control FPS and Game Update Rate
const int TICKS_PER_SECOND = 50;
const float SKIP_TICKS = 1.0 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 10;

// Possible camere view modes
enum ViewModes
{
    TOP_TOWER,
    BEHIND,
    SIDE_VIEW,
    TOP_VIEW
};

// Camera related variables
const float DELTA_ANGLE = Util::PI / 36;
glm::vec3 center(0,0,0);
glm::vec3 up(0,1,0);
glm::vec3 eye(0,0,10);
float aspectRatio = 800.0/600.0;
ViewModes cameraMode = TOP_VIEW;

// The Floor
glm::vec4 floorColor(1.0, 1.0, 0.0, 1.0);
glm::mat4 floorPos(
    -10.0, 0.0, 10.0, 1.0,
    -10.0, 0.0, -10.0, 1.0,
    10.0, 0.0, -10.0, 1.0,
    10.0, 0.0, 10.0, 1.0
);

// Possible Leader Modes
enum LeaderModes
{
    RANDOM_POSITION,
    CIRCLE
};

// The leader
Boid* leader = 0;
LeaderModes leaderMode = CIRCLE;
glm::vec3 leaderGoal(5,5,5);
int lastUpdated = 0;

// The flock of boids
Flock* flock = 0;

// Pause and print game!
bool paused = false;

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
    glfwSetWindowSizeCallback(resized);

    // Enable Depth Test, Lighing, Lights ource and Color Material
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_COLOR_MATERIAL);

    // Inform the boids about the FPS
    Boid::setFPS(TICKS_PER_SECOND);

    // Create the Leader
    leader = new Boid(glm::vec3(5,10,5));
    leader->setVelocity(glm::vec3(0,0,-1));

    // Create the flock
    flock = new Flock(100);

    //exit(0);
}

// ============================================= //

void update()
{
    // Update the leader
    if (leaderMode == RANDOM_POSITION)
    {
        int current = (int) glfwGetTime();
        if (lastUpdated != current && current % 4 == 0)
        {
            leaderGoal.x = Util::getRandom() * 40 - 20;
            leaderGoal.y = Util::getRandom() * 20;
            leaderGoal.z = Util::getRandom() * 40 - 20;
            lastUpdated = current;
        }
        if (leader != 0) leader->update(leaderGoal);
    }
    else
    {
        leaderGoal = glm::vec3(50, 10, 50);
        leaderGoal.x *= glm::cos(glfwGetTime() * Util::PI / 8);
        leaderGoal.z *= -glm::sin(glfwGetTime() * Util::PI / 8);
        if (leader != 0) leader->update(leaderGoal);
    }

    // Update the flock
    glm::vec3 leaderHeading = leader->getHeading();
    leaderHeading *= 5;
    glm::vec3 leaderPos = leader->getPosition();
    if (flock != 0) flock->update( leaderPos - leaderHeading );

    // Update Camera position
    if (cameraMode == BEHIND)
    {
        glm::vec3 dist = flock->computeFlockHeading();
        dist *= 10;
        eye = flock->computeFlockCenter() - dist;
        center = flock->computeFlockCenter();
    }
    else if (cameraMode == SIDE_VIEW)
    {
        up = glm::vec3(0,1,0);
        glm::vec3 vetor = glm::cross(up, flock->computeFlockHeading());
        vetor /= glm::length(vetor);
        vetor *= 15;
        eye = flock->computeFlockCenter() - vetor;
        center = flock->computeFlockCenter();
    }
    else if (cameraMode == TOP_TOWER)
    {
        eye = glm::vec3(0, 25, 0);
        up = glm::vec3(0,1,0);
        center = flock->computeFlockCenter();
    }
    else
    {
        eye = glm::vec3(0, 50, 0);
        up = glm::vec3(0,0,-1);
        center = glm::vec3(0, 0, 0);
    }

/*
    eye = glm::vec3(20, 3, 20);
    up = glm::vec3(0,1,0);
    center = glm::vec3(0, 0, 0);
*/

}

// ============================================= //

void draw()
{
    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

    // Set Up Light Source
    GLfloat lightPos[] = {5.0, 5.0, 5.0, 1.0f};
    GLfloat ltAmbColor[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat ltDifColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ltSpeColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ltAmbColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ltDifColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ltSpeColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Set Perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspectRatio, 1.6, 100);
    glMatrixMode(GL_MODELVIEW);

    // Draw the leader
    if (leader != 0) leader->draw();

    // Draw the flock
    if (flock != 0) flock->draw();

    // Draw the Floor
    glBegin(GL_QUADS);
        glColor4f(0.2, 0.7, 0.0, 1.0);
        glNormal3f(0, 1, 0);
        glVertex3f(eye.x-100, 0, eye.z+100);
        glVertex3f(eye.x-100, 0, eye.z-100);
        glVertex3f(eye.x+100, 0, eye.z-100);
        glVertex3f(eye.x+100, 0, eye.z+100);
    glEnd();

    // Draw Tower
    glBegin(GL_TRIANGLE_FAN);
        glColor4f(0.7, 0.7, 0.7, 1.0);
        glNormal3f(0, 1, 0);
        glVertex3f(0, 25, 0);

        int numSteps = 36;
        float angleInc = 2*Util::PI/numSteps;
        for (auto i = 0; i <= numSteps; i++ )
        {
            float angle = i*angleInc;
            //float nextAngle = (i+1)*angleInc;
            glm::vec3 pos (glm::cos(angle)*2, 0, -glm::sin(angle)*2);
            glNormal3f(pos.x, 0, pos.z);
            glVertex3f(pos.x, pos.y, pos.z);
        }

    glEnd();


    glfwSwapBuffers();
}

// ============================================= //

void run()
{
    std::cout << "Starting Main Loop." << std::endl;
    glClearColor(0.85, 0.85, 1.0, 1.0);

    double nextGameTick = glfwGetTime();
    int loops;
    bool running = true;
    while( running ) {

        /* Update the simulation at rate defined in TICS_PER_SECOND ,
         * Allowing a max frameskip rate defined at MAX_FRAMESKIP
         */
        loops = 0;
        while( glfwGetTime() > nextGameTick && loops < MAX_FRAMESKIP) {
            if (!paused) update();
            nextGameTick += SKIP_TICKS;
            loops++;
        }

        // Draws the scene, as fast as possible
        if (!paused) draw();

        // Capture Input Events
        glfwPollEvents();

        // Check for ESC key or windows Close
        if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS ||
            !glfwGetWindowParam(GLFW_OPENED))
        {
            running = false;
        }
    }
}

// ============================================= //

void print()
{
    std::cout << "--------------------" << std::endl;
    std::cout << "Eye: (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
    std::cout << "Center: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
    std::cout << "Leader: (" << leader->getPosition().x << ", " << leader->getPosition().y << ", " << leader->getPosition().z << ")" << std::endl;
    std::cout << "Leader Goal: (" << leaderGoal.x << ", " << leaderGoal.y << ", " << leaderGoal.z << ")" << std::endl;
    std::cout << "Num. Boids: " << flock->getNumBoids() << std::endl;
    std::cout << "--------------------" << std::endl;
}

// ============================================= //

void resized(int width, int height)
{
    glViewport(0,0, width, height);
    aspectRatio = (float) width / (float) height;
}

// ============================================= //

void keyPressed(int key, int status)
{
    if(status == GLFW_RELEASE) return;

    if (key == 'P') paused = !paused;
    else if (key == 'N' && paused)
    {
        update(); draw();
        print();
    }
    else if (key == GLFW_KEY_KP_ADD) flock->addBoid();
    else if (key == GLFW_KEY_KP_SUBTRACT) flock->deleteBoid();
    else if (key == GLFW_KEY_KP_7) cameraMode = TOP_VIEW;
    else if (key == GLFW_KEY_KP_5) cameraMode = TOP_TOWER;
    else if (key == GLFW_KEY_KP_1) cameraMode = BEHIND;
    else if (key == GLFW_KEY_KP_3) cameraMode = SIDE_VIEW;
    else if (key == GLFW_KEY_F1) leaderMode = RANDOM_POSITION;
    else if (key == GLFW_KEY_F2) leaderMode = CIRCLE;
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
