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
    this->m_Speed = 0;

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
    glm::vec3 heading;
    heading.x = glm::cos(m_AngleX)*glm::sin(m_AngleY);
    heading.y = glm::sin(m_AngleX);
    heading.z = -glm::cos(m_AngleX)*glm::cos(m_AngleY);
    return (heading);
}

// ============================================= //

void Boid::setSpeed(double speed)
{
    m_Speed = speed;
}

// ============================================= //

void Boid::addSpeed(double speed)
{
    m_Speed += speed;
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
    m_AngleX += degrees/180*Util::PI;
    if (m_AngleX > Util::PI/2) m_AngleX = Util::PI/2;
    if (m_AngleX < -Util::PI/2) m_AngleX = -Util::PI/2;

}

// ============================================= //

void Boid::rotateYaw(double degrees)
{
    m_AngleY += degrees/180*Util::PI;
    if (m_AngleX > Util::PI*2) m_AngleX -= 2*Util::PI;
    if (m_AngleX < 0) m_AngleX += 2*Util::PI;
}

// ============================================= //

void Boid::update()
{
    glm::vec3 target = getHeading();
    target *= m_Speed;

    myStep(
        glm::vec3(0,0,0), // Separation Component
        glm::vec3(0,0,0), // Alignment Component
        glm::vec3(0,0,0), // Cohesion Component
        target, //glm::vec3(0,0,0), // Target Component
        glm::vec3(0,0,0), // Floor Component
        glm::vec3(0,0,0)  // Tower Component
    );
}

// ============================================= //

void Boid::update(glm::vec3 target)
{
    // Compute target component
    glm::vec3 targetComp = (target - m_Position);
    targetComp = Util::normalize(targetComp, 1.5);

    myStep(
        glm::vec3(0,0,0), // Separation Component
        glm::vec3(0,0,0), // Alignment Component
        glm::vec3(0,0,0), // Cohesion Component
        targetComp,       // Target Component
        glm::vec3(0,0,0), // Floor Component
        glm::vec3(0,0,0)  // Tower Component
    );
}

// ============================================= //

void Boid::update(glm::vec3 separation, glm::vec3 flockVelocity,
                  glm::vec3 center, glm::vec3 target)
{
    // Compute alignment component
    glm::vec3 heading = getHeading();
    glm::vec3 alignmentComp = (flockVelocity - heading);

    // Compute cohesion component
    glm::vec3 cohesionComp = (center - m_Position);
    cohesionComp = Util::normalize(cohesionComp, 2.5);

    // Compute separation component
    glm::vec3 separationComp = separation;

    // Compute target component
    glm::vec3 targetComp = (target - m_Position);
    targetComp = Util::normalize(targetComp, 1.5);

    // Compute floor component
    glm::vec3 floorComp = glm::vec3(0, 1, 0);
    floorComp *= Util::normNegSigmoid(m_Position.y, 1.5);

    // Compute tower component
    glm::vec3 towerComp(0,0,0);
    if (m_Position.y < 25)
    {
        towerComp = m_Position - glm::vec3(0, m_Position.y, 0);
        towerComp *= Util::normalizeNeg(towerComp, 5);
        towerComp *= 10;
    }

    // Step in
    myStep(separationComp, alignmentComp, cohesionComp, targetComp, floorComp, towerComp);
}

// ============================================= //

void Boid::myStep(glm::vec3 separationComp, glm::vec3 alignmentComp,
                  glm::vec3 cohesionComp, glm::vec3 targetComp,
                  glm::vec3 floorComp, glm::vec3 towerComp)
{
    // Convert Angles to Heading
    glm::vec3 heading;
    heading.x = glm::cos(m_AngleX)*glm::sin(m_AngleY);
    heading.y = glm::sin(m_AngleX);
    heading.z = -glm::cos(m_AngleX)*glm::cos(m_AngleY);


    // Some factors
    float alignmentFactor = 1;
    float cohesionFactor = 1;
    float separationFactor = 1;
    float targetFactor = 1;
    float floorFactor = 1;
    float towerFactor = 1;
    float sumOfFactors = (
        alignmentFactor + cohesionFactor + separationFactor +
        targetFactor + floorFactor + towerFactor
    );

    // Apply factors the the components
    alignmentComp *= alignmentFactor;
    cohesionComp *= cohesionFactor;
    separationComp *= separationFactor;
    targetComp *= targetFactor;
    floorComp *= floorFactor;
    towerComp *= towerFactor;

    // Compute resulting Velocity and speed
    glm::vec3 velocity = separationComp + alignmentComp + cohesionComp +
        targetComp + floorComp + towerComp;
    velocity/= sumOfFactors;
    m_Speed = glm::length(velocity);

    // Update the heading
    // H' = <H + <V>>
    if (glm::length(glm::cross(heading, velocity)) < 0.0001) velocity += 0.0001;
    glm::vec3 newHeading = Util::normalize(heading + Util::normalize(velocity));

    // Ensure Max Speed
    double MaxSpeed = 5.0 / s_FPS;
    if (m_Speed > MaxSpeed) m_Speed = MaxSpeed;

    // Update position
    glm::vec3 step = newHeading;
    step *= m_Speed;
    m_Position += step;

    // Compute m_AngleX
    m_AngleX = glm::asin(newHeading.y);

    // Compute m_AngleY
    glm::vec3 newHeadProjXZ = newHeading * glm::vec3(1,0,1);
    glm::vec3 headProjXZ = heading * glm::vec3(1,0,1);
    if (glm::length(glm::cross(newHeadProjXZ, headProjXZ)) < 0.0001)
    {
        headProjXZ.x += 0.0001;
        headProjXZ.z += 0.0001;
    }
    double turnAngle = Util::computeAngle(newHeadProjXZ, headProjXZ);
    if (glm::cross(newHeadProjXZ, headProjXZ).y < 0)
    {
        m_AngleY -= turnAngle* Util::PI / 180;
    }
    else
    {
        m_AngleY += turnAngle* Util::PI / 180;
    }
    if (m_AngleY > 2*Util::PI) m_AngleY -= 2*Util::PI;
    if (m_AngleY < 0) m_AngleY += 2*Util::PI;

    // Compute Z Axis Rotation Angle (Roll or bank)
    double curveAngle = turnAngle * 10;
    m_AngleZ = curveAngle / 180 * 90; // Cap max roll at 90 degrees
    m_AngleZ = m_AngleZ/180*Util::PI;
    //std::cout << "Curve: " << curveAngle << std::endl;
    //std::cout << "Turn: " << turnAngle << std::endl;

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

void Boid::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(m_Position.x, m_Position.y, m_Position.z);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(m_AngleX * 180 / Util::PI, 1, 0, 0);
    glRotatef(-m_AngleY * 180 / Util::PI, 0, 1, 0);
    glRotatef(m_AngleZ * 180 / Util::PI, 0, 0, 1);

    float boidSpec[] = {0.1, 0.1, 0.1, 1.0};
    float boidEmis[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, boidSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, boidEmis);
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
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glVertex3f(-m_NeckSize/2,m_NeckSize/2,-(1-m_HeadHeight));
        glColor3f(1.0, 1.0, 0.0);

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
