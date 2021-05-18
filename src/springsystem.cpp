// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include "springsystem.h"

using namespace std;
using namespace agl;
using namespace glm;

SpringSystem::~SpringSystem()
{
    mParticles.clear();
    mSprings.clear();
}

void SpringSystem::createParticles(int size)
{
    mTexture = theRenderer.loadTexture("../textures/particle.png");
    //for (int i = 0; i < size; i++)
    //{
    Particle p1 = {vec3(0.0, 0.5, 0.0), vec3(0), vec3(0), vec4(1), 0.2, 1.0}; //fixed
    Particle p2 = {vec3(0.0, 0.0, 0.0), vec3(0), vec3(0), vec4(vec3(0, 0, 1), 1), 0.2, 0.5};
    Particle p3 = {vec3(-0.5, 0.5, -6), vec3(0), vec3(0), vec4(vec3(0, 1, 1), 1), 0.2, 1.0};
    Particle p4 = {vec3(0.5, 0.5, 0), vec3(0), vec3(0), vec4(vec3(0, 0, 1), 1), 0.2, 1.0};
    //Particle p5 = {vec3(0.5, -0.5, 0), vec3(0), vec3(0), vec4(vec3(0, 0, 1), 1), 1.0, 1.0};

    mParticles.push_back(p1);
    mParticles.push_back(p2);
    mParticles.push_back(p3);
    mParticles.push_back(p4);
    //mParticles.push_back(p5);

    Spring s1 = Spring{0, 1};

    Spring s2 = {0, 2};
    Spring s3 = {1, 2};
    Spring s4 = {2, 3};

    mSprings.push_back(s1);

    mSprings.push_back(s2);
    mSprings.push_back(s3);
    mSprings.push_back(s4);

    //}
}

bool SpringSystem::moveParticle(double xpos, double ypos)
{
    for (int i = 0; i < mParticles.size(); i++)
    {
        vec3 worldPos = mParticles[i].pos;
        vec2 screenPos = theRenderer.getScreenCoords(worldPos);
        // temporarily using 30 as in range
        bool inRangeX = (screenPos.x - 30 < xpos) && (xpos < screenPos.x + 30);
        bool inRangeY = (screenPos.y - 30 < ypos) && (ypos < screenPos.y + 30);
        if (inRangeX && inRangeY)
        {
            vec2 screen2D = vec2(xpos, ypos);
            vec3 newWorldPos = theRenderer.getWorldCoords(screen2D);

            mParticles[i].pos = newWorldPos;
            mParticles[i].vel = vec3(0);
            return true;
        }

        /*bool inRangeX = (mParticles[i].pos.x - mParticles[i].size / 2.0 < xpos) && (xpos < mParticles[i].pos.x + mParticles[i].size / 2.0);
        bool inRangeY = (mParticles[i].pos.y - mParticles[i].size / 2.0 < ypos) && (ypos < mParticles[i].pos.y + mParticles[i].size / 2.0);

        if (inRangeX && inRangeY)
        {
            mParticles[i].pos = vec3(xpos, ypos, 0);
            mParticles[i].vel = vec3(0);
            return true;
        }*/
    }
    return false;
}

void SpringSystem::calculateForces(float dt)
{
    //vec3 gravity = vec3(0, -1.0f, 0);
    vec3 gravity = vec3(0);
    for (int i = 1; i < mParticles.size(); i++)
    {
        mParticles[i].force = gravity * mParticles[i].mass;
    }
    float r = 0.0f;

    float kspring = 6.0f;

    float kdamp = 0.05f;

    for (int i = 0; i < mSprings.size(); i++)
    {
        Particle p1 = mParticles[mSprings[i].index1];
        Particle p2 = mParticles[mSprings[i].index2];

        vec3 deltaX = p2.pos - p1.pos;
        vec3 v1 = p1.vel;
        vec3 v2 = p2.vel;

        float l = length(deltaX);
        vec3 force = -kspring * ((length(deltaX) - r) + kdamp * dot(v2 - v1, normalize(deltaX))) * normalize(deltaX);
        p1.force -= force;
        p2.force += force;
        mParticles[mSprings[i].index1] = p1;
        mParticles[mSprings[i].index2] = p2;
    }
}

void SpringSystem::draw()
{
    theRenderer.begin(mTexture, mBlendMode);
    for (int i = 0; i < mParticles.size(); i++)
    {
        Particle particle = mParticles[i];
        theRenderer.quad(particle.pos, particle.color, particle.size);
    }

    for (int i = 0; i < mSprings.size(); i++)
    {
        Particle p1 = mParticles[mSprings[i].index1];
        Particle p2 = mParticles[mSprings[i].index2];

        theRenderer.line(p1.pos, p2.pos, vec4(1));
    }
}

void SpringSystem::update(float dt)
{
    for (int i = 1; i < mParticles.size(); i++)
    {
        if (i == 3)
            continue;
        vec3 vel = mParticles[i].vel;
        vec3 pos = mParticles[i].pos;
        vec3 force = mParticles[i].force;
        float mass = mParticles[i].mass;

        vel += dt * (force / mass);
        pos += dt * vel;

        mParticles[i].vel = vel;
        mParticles[i].pos = pos;
    }
}