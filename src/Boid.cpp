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

    this->m_AngleX = 0;
    this->m_AngleY = 0;
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

glm::vec3 Boid::getVelocity()
{
    return (this->m_Velocity);
}

// ============================================= //

glm::vec3 Boid::getHeading()
{
    return (this->m_Heading);
}

// ============================================= //

glm::vec3 Boid::computeSeparation(Boid& b)
{
    // Compute the separation between the two boids
    glm::vec3 sep = (b.getPosition() - m_Position);

    // Normalize it by a distance factor
    sep = Util::normalizeNeg(sep, 2.5);

    // Normalize it and return
    return -sep;
}

// ============================================= //

void Boid::update(glm::vec3 separation, glm::vec3 flockVelocity,
                  glm::vec3 center, glm::vec3 target)
{
    /* Some factors */
    float alignmentFactor = 1;
    float cohesionFactor = 1;
    float separationFactor = 1;
    float targetFactor = 1;

    /* Compute alignment component */
    glm::vec3 alignmentComp = (flockVelocity - m_Velocity);
    alignmentComp *= alignmentFactor;

    /* Compute cohesion component */
    glm::vec3 cohesionComp = (center - m_Position);
    cohesionComp = Util::normalize(cohesionComp, 2.5);
    cohesionComp *= cohesionFactor;

    /* Compute separation component */
    glm::vec3 separationComp = separation;
    separationComp *= separationFactor;

    /* Compute target component */
    glm::vec3 targetComp = (target - m_Position);
    targetComp = Util::normalize(targetComp, 1.5);
    targetComp *= targetFactor;

    /* Update Velocity */
    glm::vec3 sum = separationComp + alignmentComp + cohesionComp + targetComp;
    sum /= (alignmentFactor + cohesionFactor + separationFactor + targetFactor);
    m_Velocity = sum;

    /* Ensure Max Speed */
    if (glm::length(m_Velocity) > 0.5)
    {
        m_Velocity = glm::normalize(m_Velocity);
        m_Velocity *= 0.5;
    }

    /* Update Heading based on new velocity */
    m_Heading = glm::normalize(m_Heading + m_Velocity);
    glm::vec3 aux = m_Heading;
    glm::vec3 origHeading(0,0,-1);

    // Compute Y Axix Rotation Angle
    aux = m_Heading;
    aux.y = 0;
    double y = glm::cross(origHeading, aux).y;
    m_AngleY = Util::computeAngle(origHeading, aux);
    m_AngleY *= y / glm::abs(y);

    // Compute X Axix Rotation Angle
    aux = m_Heading;
    aux.x = 0;
    double x = glm::cross(origHeading, aux).x;
    m_AngleX = Util::computeAngle(origHeading, aux);
    m_AngleX *= x / glm::abs(x);

    /* Update Up based on Velocity */
    //m_Up = mUp - m_Velocity;

    /* Update Position based on new velocity */
    m_Position += m_Velocity;

    /* Compute Wing flapping */
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
    //glRotatef(-90,0,0,1);
    glRotatef(m_AngleY,0,1,0);
    glRotatef(m_AngleX,1,0,0);

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
