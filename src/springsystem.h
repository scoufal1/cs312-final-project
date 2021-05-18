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
        bool moveParticle(double xpos, double ypos);
        void calculateForces(float dt);

    protected:
        std::vector<Spring> mSprings;
    };
} // namespace agl
#endif