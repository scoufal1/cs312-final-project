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

   class SimpleSpringSystem : public ParticleSystem
   {
   public:
      virtual void createParticles(int size) override;
      virtual void update(float dt) override;
      virtual void draw() override;
      void moveParticle(double xpos, double ypos);
      void calculateForces(float dt);
   };
} // namespace agl
#endif