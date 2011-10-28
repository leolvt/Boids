#include <GL/gl.h>
#include <GL/glfw.h>

#include "Boid.h"

#define PI 3.141592653589793

namespace Boids {

// ============================================= //

Boid::Boid(glm::vec3 pos, bool rotate)
{
    this->m_Position = pos;
    this->m_Rotate = rotate;

    this->m_NeckSize = 0.3 * 2;
    this->m_HeadHeight = 0.2 * 2;
    this->m_BodyHeight = 0.6 * 2;
    this->m_TailHeight = 0.2 * 2;
    this->m_TailWidth = 0.05 * 2;
    this->m_TailLength = 0.3 * 2;
    this->m_WingWidth = 0.05 * 2;
    this->m_WingLength = 0.7 * 2;
    this->m_WingTipHeight = 0.2 * 2;
    this->m_WingTipFlap = 0;
}

// ============================================= //

Boid::~Boid()
{

}

// ============================================= //

void Boid::update()
{
    this->m_WingTipFlap = glm::cos(glfwGetTime()*PI);
}

// ============================================= //

void Boid::draw()
{
    if (m_Rotate)
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(m_Position.x,m_Position.y, m_Position.z);
        glRotated(-90,1,0,0);
    }

    glPushMatrix();
    //glRotatef(45.0, 0, 1, 0);

    // Draw Head
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, m_NeckSize/2);
        glVertex3f(m_NeckSize/2, 1-m_HeadHeight, m_NeckSize/2);
        glVertex3f(m_NeckSize/2, 1-m_HeadHeight, -m_NeckSize/2);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, -m_NeckSize/2);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, m_NeckSize/2);
    glEnd();

    // Draw Body
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(0.0, 1.0-m_HeadHeight-m_BodyHeight, 0.0);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, m_NeckSize/2);
        glVertex3f(m_NeckSize/2, 1-m_HeadHeight, m_NeckSize/2);
        glVertex3f(m_NeckSize/2, 1-m_HeadHeight, -m_NeckSize/2);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, -m_NeckSize/2);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, m_NeckSize/2);
    glEnd();
    glPopMatrix();

    // Draw Tail
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 0.5, 0.0);
        glVertex3f(0.0, 1.0-m_HeadHeight-m_BodyHeight, 0.0);
        glVertex3f(-m_TailLength/2, 1-m_HeadHeight-m_BodyHeight-m_TailHeight, m_TailWidth/2);
        glVertex3f(m_TailLength/2, 1-m_HeadHeight-m_BodyHeight-m_TailHeight, m_TailWidth/2);
        glVertex3f(m_TailLength/2, 1-m_HeadHeight-m_BodyHeight-m_TailHeight, -m_TailWidth/2);
        glVertex3f(-m_TailLength/2, 1-m_HeadHeight-m_BodyHeight-m_TailHeight, -m_TailWidth/2);
        glVertex3f(-m_TailLength/2, 1-m_HeadHeight-m_BodyHeight-m_TailHeight, m_TailWidth/2);
    glEnd();

    // Draw Right Wing
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.2, 1.0, 0.0);
        glVertex3f(m_WingLength, 1-m_HeadHeight-m_BodyHeight+m_WingTipHeight, m_WingTipFlap);
        glVertex3f(m_NeckSize/2, 1-m_HeadHeight, m_WingWidth/2);
        glVertex3f(m_NeckSize/2, 1-m_HeadHeight, -m_WingWidth/2);
        glVertex3f(0.0, 1.0-m_HeadHeight-m_BodyHeight, 0.0);
        glVertex3f(m_NeckSize/2, 1-m_HeadHeight, m_WingWidth/2);
    glEnd();

    // Draw Left Wing
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.2, 1.0, 0.0);
        glVertex3f(-m_WingLength, 1-m_HeadHeight-m_BodyHeight+m_WingTipHeight, m_WingTipFlap);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, m_WingWidth/2);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, -m_WingWidth/2);
        glVertex3f(0.0, 1.0-m_HeadHeight-m_BodyHeight, 0.0);
        glVertex3f(-m_NeckSize/2, 1-m_HeadHeight, m_WingWidth/2);
    glEnd();

    if (m_Rotate) glPopMatrix();
}

// ============================================= //

};
