#ifndef FLOCK_H_INCLUDED
#define FLOCK_H_INCLUDED

#include <vector>

#include <glm/glm.hpp>

#include "Boid.h"

namespace Boids {

// ============================================= //

class Flock
{
    public:
        Flock(unsigned int numBoids);
        virtual ~Flock();
        void update();
        void draw();
    private:
        glm::vec3 computeFlockCenter();
        glm::vec3 computeFlockHeading();
        glm::vec3 computeFlockVelocity();
        glm::vec3 computeBoidSeparation(std::vector<Boid>::iterator currBoid);

        unsigned int m_NumBoids;
        std::vector<Boid> boids;
};

// ============================================= //

};

#endif // FLOCK_H_INCLUDED
