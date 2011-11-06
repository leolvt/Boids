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
        void update(glm::vec3 target);
        void update(glm::vec3 separation, glm::vec3 flockVelocity,
                    glm::vec3 center, glm::vec3 target);

        void draw();

        glm::vec3 getPosition();
        glm::vec3 getHeading();
        glm::vec3 getVelocity();

        void setVelocity(glm::vec3 newVelocity);
        glm::vec3 computeSeparation(Boid& b);

        void rotateYaw(double degrees);
        void rotatePitch(double degrees);

        static void setFPS(unsigned int FPS);

    private:
        void step(glm::vec3 separationComp, glm::vec3 alignmentComp,
                 glm::vec3 cohesionComp, glm::vec3 targetComp,
                 glm::vec3 floorComp, glm::vec3 towerComp);

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

        double m_Yaw_Constant_Rotation;
        double m_Pitch_Constant_Rotation;

        static unsigned int s_FPS;
};

// ============================================= //

};

#endif // BOID_H_INCLUDED
