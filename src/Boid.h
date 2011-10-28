#ifndef BOID_H_INCLUDED
#define BOID_H_INCLUDED

#include "Drawable.h"

namespace Boids
{

// ============================================= //

class Boid : public Drawable
{
    public:
        Boid(glm::vec3 position, bool rotate);
        virtual ~Boid();
        void update();
        void draw();

    private:
        glm::vec3 m_Position;
        bool m_Rotate;
};

// ============================================= //

};

#endif // BOID_H_INCLUDED
