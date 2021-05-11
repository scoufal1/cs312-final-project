#ifndef MyParticleSystem_H_
#define MyParticleSystem_H_

#include "particlesystem.h"

namespace agl
{

   struct Spring
   {
      Particle p1;
      Particle p2;
   };

   class SpringSystem : public ParticleSystem
   {
   public:
      virtual void createParticles(int size) override;
      virtual void update(float dt) override;
   };
} // namespace agl
#endif