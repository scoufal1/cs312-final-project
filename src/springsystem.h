#ifndef MyParticleSystem_H_
#define MyParticleSystem_H_

#include "particlesystem.h"

namespace agl
{

    struct Spring
    {
        int index1;
        int index2;
    };

    class SpringSystem : public ParticleSystem
    {
    public:
        virtual void createParticles(int size) override;
        virtual void update(float dt) override;
        virtual void draw() override;
        virtual ~SpringSystem() override;
        int hitParticle(double xpos, double ypos);
        bool moveParticle(int selectedParticle, double xpos, double ypos);
        void calculateForces(float dt);
        void increaseKspring();
        void decreaseKspring();
        void increaseMasses();
        void decreaseMasses();

    protected:
        std::vector<Spring> mSprings;
        float kspring = 6.0f;
    };
} // namespace agl
#endif