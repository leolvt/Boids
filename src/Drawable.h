#ifndef DRAWABLE_H_INCLUDED
#define DRAWABLE_H_INCLUDED

#include <glm/glm.hpp>

namespace Boids
{

// ============================================= //

class Drawable
{
    public:
        virtual void update() = 0;
        virtual void draw() = 0;
};

// ============================================= //

};

#endif // DRAWABLE_H_INCLUDED
