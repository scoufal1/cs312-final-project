#ifndef ParticleSystem_H_
#define ParticleSystem_H_

#include "AGL.h"
#include "image.h"
#include "renderer.h"
#include <vector>

namespace agl
{

   struct Particle
   {
      glm::vec3 pos;
      glm::vec3 vel;
      glm::vec3 force;
      glm::vec4 color;
      float size;
      float mass;
   };

   class ParticleSystem
   {
   public:
      ParticleSystem();
      virtual ~ParticleSystem();

      void init(int size);
      virtual void update(float dt) = 0;
      virtual void draw();

      static Renderer &GetRenderer();
      std::vector<Particle> mParticles;

   protected:
      virtual void createParticles(int size) = 0;

   protected:
      GLuint mTexture;
      BlendMode mBlendMode;
      static Renderer theRenderer;
   };
} // namespace agl
#endif