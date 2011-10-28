#include <iostream>
#include <GL/glu.h>
#include <GL/glfw.h>
#include <cmath>

#define PI 3.141592653589793

const float angleInc = PI / 36;
float eyeX = 0;
float eyeY = 0;
float eyeZ = 5;
float upX = 0;
float upY = 1;
float upZ = 0;
float eyeRadius = 5;
float eyeTheta = 0.0;
float eyePhi = 0.0;

void draw()
{
    // Clear Screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ,   0, 0, 0,   upX, upY, upZ);

    // Set Perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.33, 1.6, 10);
    glMatrixMode(GL_MODELVIEW);

    // Draw Faces Far and Near
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.8, 0.8, -0.8);
    glVertex3f(0.8, -0.8, -0.8);
    glVertex3f(-0.8, -0.8, -0.8);
    glVertex3f(-0.8, 0.8, -0.8);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.8, 0.8, 0.8);
    glVertex3f(0.8, -0.8, 0.8);
    glVertex3f(-0.8, -0.8, 0.8);
    glVertex3f(-0.8, 0.8, 0.8);
    glEnd();

    // Draw Faces Right and Left
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.8, -0.8, -0.8);
    glVertex3f(0.8, 0.8, -0.8);
    glVertex3f(0.8, 0.8, 0.8);
    glVertex3f(0.8, -0.8, 0.8);

    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-0.8, -0.8, -0.8);
    glVertex3f(-0.8, 0.8, -0.8);
    glVertex3f(-0.8, 0.8, 0.8);
    glVertex3f(-0.8, -0.8, 0.8);
    glEnd();

    // Draw Faces Top and Bottom
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-0.8, 0.8, -0.8);
    glVertex3f(-0.8, 0.8, 0.8);
    glVertex3f(0.8, 0.8, 0.8);
    glVertex3f(0.8, 0.8, -0.8);

    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-0.8, -0.8, -0.8);
    glVertex3f(-0.8, -0.8, 0.8);
    glVertex3f(0.8, -0.8, 0.8);
    glVertex3f(0.8, -0.8, -0.8);
    glEnd();

    glfwSwapBuffers();
}

void keyPressed(int key, int status)
{
    if(status == GLFW_RELEASE) return;
    if(key == GLFW_KEY_LEFT)
    {
        eyePhi -= angleInc;
        if(eyePhi < 0) eyePhi += 2 * PI;
        if(eyePhi > 2 * PI) eyePhi -= 2 * PI;
    }
    else if(key == GLFW_KEY_RIGHT)
    {
        eyePhi += angleInc;
        if(eyePhi < 0) eyePhi += 2 * PI;
        if(eyePhi > 2 * PI) eyePhi -= 2 * PI;
    }
    else if(key == GLFW_KEY_UP)
    {
        eyeTheta += angleInc;
        if(eyeTheta < -PI / 2) eyeTheta = -PI / 2;
        if(eyeTheta > PI / 2) eyeTheta = PI / 2;
    }
    else if(key == GLFW_KEY_DOWN)
    {
        eyeTheta -= angleInc;
        if(eyeTheta < -PI / 2) eyeTheta = -PI / 2;
        if(eyeTheta > PI / 2) eyeTheta = PI / 2;
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

int main()
{
    std::cout << "Starting GLFW and opening window." << std::endl;
    glfwInit();
    glfwOpenWindow(800, 600, 8, 8, 8, 8, 8, 0, GLFW_WINDOW);
    glfwSetKeyCallback(keyPressed);
    glEnable(GL_DEPTH_TEST);
    std::cout << "Starting GLEW." << std::endl;

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
    return 0;
}
