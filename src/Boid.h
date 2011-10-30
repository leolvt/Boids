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
        void update(glm::vec3 separation, glm::vec3 flockVelocity,
                    glm::vec3 center, glm::vec3 target);
        void draw();
        glm::vec3 getPosition();
        glm::vec3 getHeading();
        glm::vec3 getVelocity();
        glm::vec3 computeSeparation(Boid& b);

        static void setFPS(unsigned int FPS);

    private:
        glm::vec3 m_Position;
        glm::vec3 m_Heading;
        glm::vec3 m_Velocity;

        float m_AngleX;
        float m_AngleY;
        float m_AngleZ;
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
        float m_FlapPhase;
        float m_FlapFactor;
        float m_FlapTick;

        static unsigned int s_FPS;
};

// ============================================= //

};

#endif // BOID_H_INCLUDED
