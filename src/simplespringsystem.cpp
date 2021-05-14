// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include "simplespringsystem.h"

using namespace std;
using namespace agl;
using namespace glm;

void SimpleSpringSystem::createParticles(int size)
{
   mTexture = theRenderer.loadTexture("../textures/particle.png");

   Particle p1 = {vec3(0, 0.5, 0), vec3(0), vec3(0), vec4(1), 0.1, 1.0}; //fixed
   Particle p2 = {vec3(0, 0, 0), vec3(0), vec3(0), vec4(vec3(0, 1, 1), 1), 0.2, 0.5};
   mParticles.push_back(p1);
   mParticles.push_back(p2);
}

void SimpleSpringSystem::moveParticle(double xpos, double ypos)
{
   int index = -1;
   bool inRangeX = (mParticles[1].pos.x - mParticles[1].size / 2.0 < xpos) && (xpos < mParticles[1].pos.x + mParticles[1].size / 2.0);
   bool inRangeY = (mParticles[1].pos.y - mParticles[1].size / 2.0 < ypos) && (ypos < mParticles[1].pos.y + mParticles[1].size / 2.0);

   if (inRangeX && inRangeY)
   {
      mParticles[1].pos = vec3(xpos, ypos, 0);
      mParticles[1].vel = vec3(0);
   }
}

void SimpleSpringSystem::calculateForces(float dt)
{
   vec3 gravity = vec3(0, -10.0f, 0);

   mParticles[1].force = gravity * mParticles[1].mass;

   float r = 0.0f;
   float kspring = 10.0f;
   float kdamp = 0.05f;
   vec3 deltaX = mParticles[1].pos - mParticles[0].pos;
   float l = length(deltaX);

   vec3 force = -kspring * ((length(deltaX) - r) + kdamp * dot(mParticles[1].vel, normalize(deltaX))) * normalize(deltaX);
   mParticles[1].force += force;
}

void SimpleSpringSystem::draw()
{
   theRenderer.begin(mTexture, mBlendMode);
   for (int i = 0; i < mParticles.size(); i++)
   {
      Particle particle = mParticles[i];
      theRenderer.quad(particle.pos, particle.color, particle.size);
   }
   theRenderer.line(mParticles[0].pos, mParticles[1].pos, mParticles[0].color);
}

void SimpleSpringSystem::update(float dt)
{
   vec3 vel = mParticles[1].vel;
   vec3 pos = mParticles[1].pos;
   vec3 force = mParticles[1].force;
   float mass = mParticles[1].mass;

   vel += dt * (force / mass);
   pos += dt * vel;

   mParticles[1].vel = vel;
   mParticles[1].pos = pos;
}