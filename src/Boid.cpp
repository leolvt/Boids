#include <GL/gl.h>

#include "Boid.h"

namespace Boids {

// ============================================= //

Boid::Boid(glm::vec3 pos, bool rotate)
{
    this->m_Position = pos;
    this->m_Rotate = rotate;
}

// ============================================= //

Boid::~Boid()
{

}

// ============================================= //

void Boid::update()
{

}

// ============================================= //

void Boid::draw()
{
    if (m_Rotate)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(m_Position.x,m_Position.y, m_Position.z);
        //glRotated(45,0,1,0);
    }

    glBegin(GL_QUADS);

    // Draw Faces Far and Near
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.8, 0.8, -0.8);
    glVertex3f(0.8, -0.8, -0.8);
    glVertex3f(0.8, -0.8, -0.8);
    glVertex3f(-0.8, 0.8, -0.8);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.8, 0.8, 0.8);
    glVertex3f(0.8, -0.8, 0.8);
    glVertex3f(-0.8, -0.8, 0.8);
    glVertex3f(-0.8, 0.8, 0.8);

    // Draw Faces Right and Left
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

    // Draw Faces Top and Bottom
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

    if (m_Rotate) glPopMatrix();
}

// ============================================= //

};
