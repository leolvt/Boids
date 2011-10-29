#ifndef FLOCK_H_INCLUDED
#define FLOCK_H_INCLUDED

#include "Drawable.h"

namespace Boids {

// ============================================= //

class Flock : public Drawable
{
    public:
        Flock(int numBoids);
        virtual ~Flock();
        void update();
        void draw();
    private:
        unsigned int m_NumBoids;
};

// ============================================= //

};

#endif // FLOCK_H_INCLUDED
