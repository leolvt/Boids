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
        double z = Util::getRandom() * 6 - 3;
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

void Flock::update()
{
    /* The vectors which influentiate the boids' speed */
    glm::vec3 separation;
    glm::vec3 alignment = computeFlockHeading();
    glm::vec3 cohesion = computeFlockCenter();

    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        separation = computeBoidSeparation(boid);
        boid->update();
    }
}

// ============================================= //

glm::vec3 Flock::computeBoidSeparation(std::vector<Boid>::iterator currBoid)
{
    glm::vec3 separation(0,0,0);
    for (auto boid = boids.begin(); boid != boids.end(); boid++)
    {
        if (boid == currBoid) continue;
        separation += (boid->getPosition() - currBoid->getPosition());
    }
    separation /= boids.size();
    return separation;
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
