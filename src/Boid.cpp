#include <iostream>

#include <GL/gl.h>
#include <GL/glfw.h>

#include "Util.h"
#include "Boid.h"

namespace Boids {

// ============================================= //

Boid::Boid(glm::vec3 pos)
{
    this->m_Position = pos;
    this->m_Velocity = glm::vec3(0, 0, 0);
    this->m_Up = glm::vec3(0, 1, 0);
    this->m_Heading = glm::vec3(0, 0, -1);

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

glm::vec3 Boid::computeSeparation(Boid& b)
{
    // Compute the separation vector and the distance between the boids
    glm::vec3 sep = b.getPosition() - m_Position;
    double dist = glm::length(sep) + 0.001;

    // Compute a normalization factor based on a sigmoid function.
    // When distance -> 0, fac -> 2,
    // when distance -> v  fac -> 0
    double v = 2.5;
    double fac = 3/(1 + glm::exp(-9/v*(v/2-dist)));

    // Return the separation vector normalized by the factor above
    sep *= fac;
    return -sep;
}

// ============================================= //

void Boid::update(glm::vec3 separation, glm::vec3 flockHeading, glm::vec3 center)
{
    /* Compute alignment component */
    glm::vec3 alignment = (flockHeading - m_Heading);
    alignment *= 0.00;

    /* Compute cohesion component */
    glm::vec3 cohesion = (center - m_Position);
    cohesion *= 0.1;

    /* Compute separation component */
    separation = (separation);
    separation *= 0.1;

    /* Update Velocity */
    glm::vec3 sum = separation + alignment + cohesion;
    std::cout << glm::length(sum) << std::endl;
    m_Velocity = sum;

    /* Ensure Max Speed */
    if (glm::length(m_Velocity) > 0.2)
    {
        m_Velocity = glm::normalize(m_Velocity);
        m_Velocity *= 0.2;
    }

    /* Update Headding based on new velocity */
    //m_Heading = glm::normalize( m_Heading + m_Velocity );

    /* Update Up based on Heading */
    //std::cout << m_Velocity.x << ", " << m_Velocity.y << ", " << m_Velocity.z << std::endl;

    /* Update Position based on new velocity */
    m_Position += m_Velocity;

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
