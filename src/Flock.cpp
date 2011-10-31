#include <iostream>

#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Flock.h"
#include "Util.h"

namespace Boids {

// ============================================= //

Flock::Flock(unsigned int numBoids)
{
    this->m_NumBoids = numBoids;
    while (numBoids-- > 0)
    {
        double x = Util::getRandom() * 6 - 3;
        double y = Util::getRandom() * 6 - 3;
        double z = Util::getRandom() * 6;
        if (x==0) x++;  if (y==0) y++;  if (z==0) z++;
        glm::vec3 pos(x,y,z);
        this->boids.push_back( Boid(pos) );
    }

}

// ============================================= //

Flock::~Flock()
{
}

// ============================================= //

void Flock::update(glm::vec3 target)
{
    /* The vectors which influentiate the boids' speed */
    glm::vec3 separation;
    glm::vec3 flockVelocity = computeFlockVelocity();
    glm::vec3 center = computeFlockCenter();

    /* Update each Boid */
    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        separation = computeBoidSeparation(boid);
        boid->update(separation, flockVelocity, center, target);
    }
}

// ============================================= //

glm::vec3 Flock::computeBoidSeparation(std::vector<Boid>::iterator currBoid)
{
    glm::vec3 separation(0,0,0);
    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        if (boid == currBoid) continue;
        separation += currBoid->computeSeparation(*boid);
    }
    return separation;
}

// ============================================= //

glm::vec3 Flock::computeFlockVelocity()
{
    glm::vec3 velocity(0,0,0);
    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        velocity += boid->getVelocity();
    }
    velocity /= boids.size();
    return velocity;
}

// ============================================= //

glm::vec3 Flock::computeFlockHeading()
{
    glm::vec3 heading(0,0,0);
    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        heading += boid->getHeading();
    }
    heading /= boids.size();
    return heading;
}

// ============================================= //

glm::vec3 Flock::computeFlockCenter()
{
    glm::vec3 center(0,0,0);
    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        center += boid->getPosition();
    }
    center /= boids.size();
    return center;
}

// ============================================= //

void Flock::draw()
{
    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        boid->draw();
    }
}

// ============================================= //

};
