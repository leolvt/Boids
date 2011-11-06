#include <iostream>

#include <GL/gl.h>
#include <GL/glfw.h>

#include "Util.h"
#include "Boid.h"

namespace Boids {

// ============================================= //

unsigned int Boid::s_FPS = 1;

// ============================================= //

void Boid::setFPS(unsigned int FPS)
{
    Boid::s_FPS = FPS;
}

// ============================================= //

Boid::Boid(glm::vec3 pos)
{
    this->m_Position = pos;
    this->m_Velocity = glm::vec3(0, 0, 0);
    this->m_Heading = glm::vec3(0, 0, -1);

    this->m_AngleX = 0;
    this->m_AngleY = 0;
    this->m_AngleZ = 0;
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
    this->m_FlapPhase = Util::getRandom()*Util::PI;
    this->m_FlapFactor = 1 + Util::getRandom();
    this->m_FlapTick = 0;

    this->m_Yaw_Constant_Rotation = 0.0;
    this->m_Pitch_Constant_Rotation = 0.0;
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

void Boid::setVelocity(glm::vec3 newVelocity)
{
    m_Velocity = newVelocity;
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

void Boid::rotatePitch(double degrees)
{
    if (degrees < -90.0) degrees = -89.0;
    if (degrees > 90.0) degrees = 89.0;
    glm::vec3 head = glm::vec3(0, 0, -1);
    head = Util::rotateAtTheta(head, degrees);
    m_Heading = head + (m_Heading - glm::vec3(0, 0, -1));

    glm::vec3 vel = m_Velocity;
    vel = Util::rotateAtTheta(vel, degrees);
    m_Velocity = vel;

    std::cout << "Degrees:" << degrees << std::endl;
    std::cout << "X:" << vel.x << std::endl;
    std::cout << "Y: " << vel.y << std::endl;
    std::cout << "Z: " << vel.z << std::endl;
}

// ============================================= //

void Boid::rotateYaw(double degrees)
{
    if (degrees < -180.0) degrees = -180.0;
    if (degrees > 180.0) degrees = 180.0;
    glm::vec3 head = glm::vec3(0, 0, -1);
    head = Util::rotateAtPhi(head, degrees);
    m_Heading = head + (m_Heading - glm::vec3(0, 0, -1));

    glm::vec3 vel = m_Velocity;
    vel = Util::rotateAtPhi(vel, degrees);
    m_Velocity = vel;
}

// ============================================= //

void Boid::update()
{
    // Ensure Max Speed
    double MaxSpeed = 5.0 / s_FPS;
    if (glm::length(m_Velocity) > MaxSpeed)
    {
        m_Velocity /= glm::length(m_Velocity) + 0.00001;
        m_Velocity *= MaxSpeed;
    }

    // Update Heading based on new velocity
    glm::vec3 oldHeading = m_Heading;
    m_Heading = m_Heading + m_Velocity;
    m_Heading /= glm::length(m_Heading) + 0.00001;
    glm::vec3 aux = m_Heading;
    glm::vec3 origHeading(0,0,-1);

    // Update Position based on new velocity
    m_Position += m_Velocity;

    // Compute Y Axix Rotation Angle (Yaw or Heading)
    aux = m_Heading;
    aux.y = 0;
    double y = glm::cross(origHeading, aux).y;
    if (y == 0) m_AngleY = 0;
    else
    {
        m_AngleY = Util::computeAngle(origHeading, aux);
        m_AngleY *= y / glm::abs(y);
    }

    // Compute X Axix Rotation Angle (Pitch or Elevation)
    aux = m_Heading;
    aux.x = 0;
    double x = glm::cross(origHeading, aux).x;
    if (x == 0) m_AngleX = 0;
    else
    {
        m_AngleX = Util::computeAngle(origHeading, aux);
        m_AngleX *= x / glm::abs(x);
    }

    // Compute Z Axis Rotation Angle (Roll or bank)
    aux = m_Heading;
    aux.y = 0;
    double yTurn = glm::cross(aux, oldHeading).y;
    double turnAngle = Util::computeAngle(m_Heading, oldHeading);
    if (yTurn != 0)
    {
        turnAngle *= -yTurn / glm::abs(yTurn);
    }
    double curveAngle = turnAngle * 50 * Util::normNegSigmoid(s_FPS,50);
    m_AngleZ = curveAngle / 180 * 90; // Cap max roll at 90 degrees

    // When not in a steep curve, flap the wings
    if (glm::abs(curveAngle) < 60.0)
    {
        m_FlapTick += 2*Util::PI / (s_FPS);
    }

    // Update the wing tip position
    float angleVariation = glm::cos(m_FlapFactor*m_FlapTick + m_FlapPhase);
    float currAngle = 30.0/180*Util::PI * angleVariation;
    m_WingTipFlapX = m_WingLength * glm::cos(currAngle);
    m_WingTipFlapZ = m_WingLength * glm::sin(currAngle);
}

// ============================================= //

void Boid::update(glm::vec3 separation, glm::vec3 flockVelocity,
                  glm::vec3 center, glm::vec3 target)
{
    // Some factors
    float alignmentFactor = 1;
    float cohesionFactor = 1;
    float separationFactor = 1;
    float targetFactor = 1;
    float sumOfFactors =
        (alignmentFactor + cohesionFactor + separationFactor + targetFactor);

    // Compute alignment component
    glm::vec3 alignmentComp = (flockVelocity - m_Velocity);
    alignmentComp *= alignmentFactor;

    // Compute cohesion component
    glm::vec3 cohesionComp = (center - m_Position);
    cohesionComp = Util::normalize(cohesionComp, 2.5);
    cohesionComp *= cohesionFactor;

    // Compute separation component
    glm::vec3 separationComp = separation;
    separationComp *= separationFactor;

    // Compute target component
    glm::vec3 targetComp = (target - m_Position);
    targetComp = Util::normalize(targetComp, 1.5);
    targetComp *= targetFactor;

    // Update Velocity
    glm::vec3 sum = separationComp + alignmentComp + cohesionComp + targetComp;
    sum /= sumOfFactors;
    glm::vec3 oldVel = m_Velocity;
    m_Velocity = sum;

    // Update normally
    update();
}

// ============================================= //

void Boid::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(m_Position.x, m_Position.y, m_Position.z);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(m_AngleX, 1, 0, 0);
    glRotatef(m_AngleY, 0, 1, 0);
    glRotatef(m_AngleZ, 0, 0, 1);
    glRotatef(180, 1, 0, 0);

    float boidSpec[] = {0.1, 0.1, 0.1, 1.0};
    float boidEmis[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, boidSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, boidEmis);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.35);
    glm::vec3 norm;

    // Draw Head
    glBegin(GL_TRIANGLES);
        glColor4f(1.0, 0.0, 0.0, 0.0);
        norm =
        glm::cross(glm::vec3(-m_NeckSize/2, m_NeckSize/2, m_HeadHeight),
                   glm::vec3(m_NeckSize/2, m_NeckSize/2, m_HeadHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0, 0.0,-(1.0));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(-m_NeckSize/2, -m_NeckSize/2, m_HeadHeight),
                   glm::vec3(-m_NeckSize/2, m_NeckSize/2, m_HeadHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0, 0.0,-(1.0));
        glVertex3f(-m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(m_NeckSize/2, m_NeckSize/2, m_HeadHeight),
                   glm::vec3(m_NeckSize/2, -m_NeckSize/2, m_HeadHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0, 0.0,-(1.0));
        glVertex3f(m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(m_NeckSize/2, -m_NeckSize/2, m_HeadHeight),
                   glm::vec3(-m_NeckSize/2, -m_NeckSize/2, m_HeadHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0, 0.0,-(1.0));
        glVertex3f(m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
    glEnd();

    // Draw Body
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 1.0, 0.0);

        norm =
        glm::cross(glm::vec3(m_NeckSize/2, m_NeckSize/2, -m_BodyHeight),
                   glm::vec3(-m_NeckSize/2, m_NeckSize/2, -m_BodyHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(-m_NeckSize/2, -m_NeckSize/2, -m_BodyHeight),
                   glm::vec3(m_NeckSize/2, -m_NeckSize/2, -m_BodyHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(-m_NeckSize/2, m_NeckSize/2, -m_BodyHeight),
                   glm::vec3(-m_NeckSize/2, -m_NeckSize/2, -m_BodyHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(m_NeckSize/2, m_NeckSize/2, -m_BodyHeight),
                   glm::vec3(m_NeckSize/2, -m_NeckSize/2, -m_BodyHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,-m_NeckSize/2,-(1-m_HeadHeight));
    glEnd();


    // Draw Tail
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.5, 0.0);

        norm =
        glm::cross(glm::vec3(-m_TailLength/2, m_TailWidth/2, m_TailHeight),
                   glm::vec3(m_TailLength/2, m_TailWidth/2, m_TailHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_TailLength/2,m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(m_TailLength/2,m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));

        norm =
        glm::cross(glm::vec3(m_TailLength/2, m_TailWidth/2, m_TailHeight),
                   glm::vec3(m_TailLength/2, -m_TailWidth/2, m_TailHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(m_TailLength/2,m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(m_TailLength/2,-m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));

        norm =
        glm::cross(glm::vec3(m_TailLength/2, -m_TailWidth/2, m_TailHeight),
                   glm::vec3(-m_TailLength/2, -m_TailWidth/2, m_TailHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(m_TailLength/2,-m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(-m_TailLength/2,-m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));

        norm =
        glm::cross(glm::vec3(-m_TailLength/2, -m_TailWidth/2, m_TailHeight),
                   glm::vec3(-m_TailLength/2, m_TailWidth/2, m_TailHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_TailLength/2,-m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));
        glVertex3f(-m_TailLength/2,m_TailWidth/2,-(1-m_HeadHeight-m_BodyHeight-m_TailHeight));

    glEnd();

    // Draw Right Wing
    glBegin(GL_TRIANGLES);
        glColor3f(0.2, 1.0, 0.0);

        norm =
        glm::cross(glm::vec3(m_NeckSize/2-m_WingTipFlapX, -m_WingWidth/2-m_WingTipFlapZ, -m_BodyHeight+m_WingTipHeight),
                   glm::vec3(m_NeckSize/2-m_WingTipFlapX, m_WingWidth/2-m_WingTipFlapZ,-m_BodyHeight+m_WingTipHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(m_NeckSize/2,-m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(-m_WingTipFlapX, -m_WingTipFlapZ, m_WingTipHeight),
                   glm::vec3(m_NeckSize/2-m_WingTipFlapX, -m_WingWidth/2-m_WingTipFlapZ, -m_BodyHeight+m_WingTipHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(m_NeckSize/2,-m_WingWidth/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(m_NeckSize/2-m_WingTipFlapX, m_WingWidth/2-m_WingTipFlapZ, -m_BodyHeight+m_WingTipHeight),
                   glm::vec3(-m_WingTipFlapX, -m_WingTipFlapZ, m_WingTipHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
    glEnd();

    // Draw Left Wing
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.2, 1.0, 0.0);

        norm =
        glm::cross(glm::vec3(-m_NeckSize/2+m_WingTipFlapX, m_WingWidth/2-m_WingTipFlapZ, -m_BodyHeight+m_WingTipHeight),
                   glm::vec3(-m_NeckSize/2+m_WingTipFlapX, -m_WingWidth/2-m_WingTipFlapZ, -m_BodyHeight+m_WingTipHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(-m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(-m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,-m_WingWidth/2,-(1-m_HeadHeight));

        norm =
        glm::cross(glm::vec3(-m_NeckSize/2+m_WingTipFlapX, -m_WingWidth/2-m_WingTipFlapZ, -m_BodyHeight+m_WingTipHeight),
                   glm::vec3(m_WingTipFlapX, -m_WingTipFlapZ, m_WingTipHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(-m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(-m_NeckSize/2,-m_WingWidth/2,-(1-m_HeadHeight));
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));

        norm =
        glm::cross(glm::vec3(m_WingTipFlapX, -m_WingTipFlapZ, m_WingTipHeight),
                   glm::vec3(-m_NeckSize/2+m_WingTipFlapX, m_WingWidth/2-m_WingTipFlapZ, -m_BodyHeight+m_WingTipHeight));
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(-m_WingTipFlapX,m_WingTipFlapZ,-(1-m_HeadHeight-m_BodyHeight+m_WingTipHeight));
        glVertex3f(0.0,0.0,-(1.0-m_HeadHeight-m_BodyHeight));
        glVertex3f(-m_NeckSize/2,m_WingWidth/2,-(1-m_HeadHeight));
    glEnd();

    glPopMatrix();
}

// ============================================= //

};
