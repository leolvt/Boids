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
        float m_NeckSize;
        float m_HeadHeight;
        float m_BodyHeight;
        float m_TailHeight;
        float m_TailWidth;
        float m_TailLength;
        float m_WingWidth;
        float m_WingLength;
        float m_WingTipHeight;
        float m_WingTipFlap;
};

// ============================================= //

};

#endif // BOID_H_INCLUDED
