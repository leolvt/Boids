#include <GL/gl.h>
#include <GL/glfw.h>

#include "Util.h"
#include "Boid.h"

namespace Boids {

// ============================================= //

Boid::Boid(glm::vec3 pos)
{
    this->m_Position = pos;

    this->m_NeckSize = 0.3 * 2;
    this->m_HeadHeight = 0.3 * 2;
    this->m_BodyHeight = 0.5 * 2;
    this->m_TailHeight = 0.2 * 2;
    this->m_TailWidth = 0.05 * 2;
    this->m_TailLength = 0.3 * 2;
    this->m_WingWidth = 0.05 * 2;
    this->m_WingLength = 0.7 * 2;
    this->m_WingTipHeight = 0.2 * 2;
    this->m_WingTipFlapX = 0.7 * 2;
    this->m_WingTipFlapZ = 0;
    this->m_Angle = 0;
    this->m_FlapPhase = Util::getRandom()*Util::PI;
    this->m_FlapFactor = 1 + Util::getRandom();
}

// ============================================= //

Boid::~Boid()
{

}

// ============================================= //

glm::vec3 Boid::getPosition()
{
    return (this->m_Position);
}

// ============================================= //

glm::vec3 Boid::getHeading()
{
    return (this->m_Heading);
}

// ============================================= //

void Boid::update()
{
    /* bewteen -1 and 1 (min and max angle) */
    float angleVariation = glm::cos(m_FlapFactor*glfwGetTime()*Util::PI + m_FlapPhase);
    float currAngle = 30.0/180*Util::PI * angleVariation;
    m_WingTipFlapX = m_WingLength * glm::cos(currAngle);
    m_WingTipFlapZ = m_WingLength * glm::sin(currAngle);

    m_Angle = 360.0 * (glfwGetTime() / 4);
}

// ============================================= //

void Boid::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    //glRotatef(m_Angle, 0,1,0);
    glTranslatef(m_Position.x,m_Position.y, m_Position.z);
    glScalef(0.5, 0.5, 0.5);
    //glRotatef(-45,1,0,0);
    //glRotatef(-90,0,0,1);
    //glRotatef(90,0,1,0);

    // Draw Head
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0,0.0,-(1.0));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
    glEnd();

    // Draw Body
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 1.0, 0.0);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
    glEnd();

    // Draw Tail
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 0.5, 0.0);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_TailLength/2,m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(m_TailLength/2,m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(m_TailLength/2,-m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(-m_TailLength/2,-m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(-m_TailLength/2,m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
    glEnd();

    // Draw Right Wing
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.2, 1.0, 0.0);
        glVertex3f(m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,-m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));
    glEnd();

    // Draw Left Wing
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.2, 1.0, 0.0);
        glVertex3f(-m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(-m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,-m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));
    glEnd();

    glPopMatrix();
}

// ============================================= //

};
