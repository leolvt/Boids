#ifndef BOID_H_INCLUDED
#define BOID_H_INCLUDED

#include <glm/glm.hpp>

namespace Boids
{

// ============================================= //

class Boid
{
    public:
        Boid(glm::vec3 position);
        virtual ~Boid();
        void update();
        void draw();
        glm::vec3 getPosition();
        glm::vec3 getHeading();

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Heading;
        glm::vec3 m_Velocity;
        glm::vec3 m_Up;

        float m_NeckSize;
        float m_HeadHeight;
        float m_BodyHeight;
        float m_TailHeight;
        float m_TailWidth;
        float m_TailLength;
        float m_WingWidth;
        float m_WingLength;
        float m_WingTipHeight;
        float m_WingTipFlapX;
        float m_WingTipFlapZ;
        float m_Angle;
        float m_FlapPhase;
        float m_FlapFactor;
};

// ============================================= //

};

#endif // BOID_H_INCLUDED
