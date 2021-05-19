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
    if (size <= 0)
    {
        cout << "Size must be greater than 0" << endl;
        exit(0);
    }
    mTexture = theRenderer.loadTexture("../textures/particle.png");
    vec4 color = vec4(random_unit_vector(), 1.0);
    float partSize = 0.15f;

    random_device rd;
    mt19937 gen(rd());                          // seed t
    uniform_real_distribution<> clr(0.0, 1.0);  // define color range
    uniform_real_distribution<> pos(-0.8, 0.8); // define position range
    for (int i = 0; i < size; i++)
    {
        color = vec4(vec3(clr(gen), clr(gen), 1), 1.0);
        partSize = 0.15f;

        // make one third of particles white and smaller
        if (i % 3 == 0)
        {
            color = vec4(1);
            partSize = 0.1f;
        }

        // construct and add particle to list
        Particle par = {vec3(pos(gen), pos(gen), pos(gen)), vec3(0), vec3(0), color, partSize, 1.0f};
        mParticles.push_back(par);

        // connect particle to previous particle with spring
        if (i > 0)
        {
            Spring s = {i - 1, i};
            mSprings.push_back(s);
        }
    }

    uniform_int_distribution<> index(0, size - 1); // define index range
    int par1;
    int par2;

    // randomly connect more particles
    for (int i = 0; i < size; i++)
    {
        par1 = index(gen);
        par2 = index(gen);
        if (par1 == par2)
        {
            continue;
        }
        mSprings.push_back({par1, par2});
    }
    // connect first and last particles
    mSprings.push_back({0, size - 1});
}

void SpringSystem::calculateForces(float dt)
{
    // calculate gravity on each particle
    vec3 gravity = vec3(0, -1.0f, 0);
    for (int i = 1; i < mParticles.size(); i++)
    {
        mParticles[i].force = gravity * mParticles[i].mass;
    }
    float r = 0.0f;
    float kdamp = 0.05f;

    // calculate spring force on both particles of each spring
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

    // copy particles into second list to sort and draw
    std::vector<Particle> mParticlesCopy;
    for (int i = 0; i < mParticles.size(); i++)
    {
        mParticlesCopy.push_back(mParticles[i]);
    }

    // sort by distance from camera
    std::sort(mParticlesCopy.begin(), mParticlesCopy.end(), [](const Particle &lhs, const Particle &rhs) {
        return distance(theRenderer.cameraPosition(), lhs.pos) > distance(theRenderer.cameraPosition(), rhs.pos);
    });

    // draw particles
    for (int i = 0; i < mParticlesCopy.size(); i++)
    {
        Particle particle = mParticlesCopy[i];
        theRenderer.quad(particle.pos, particle.color, particle.size);
    }
    mParticlesCopy.clear();

    // draw strings for each spring
    for (int i = 0; i < mSprings.size(); i++)
    {
        Particle p1 = mParticles[mSprings[i].index1];
        Particle p2 = mParticles[mSprings[i].index2];

        theRenderer.line(p1.pos, p2.pos, vec4(1));
    }
}

void SpringSystem::update(float dt)
{
    for (int i = 0; i < mParticles.size(); i++)
    {
        // fix one third of the particles
        if (i % 3 == 0)
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

void SpringSystem::increaseKspring()
{
    kspring += 0.5;
}

void SpringSystem::decreaseKspring()
{
    kspring -= 0.5;
    // limit values
    if (kspring <= 0.1)
    {
        kspring += 0.5;
    }
}

void SpringSystem::increaseMasses()
{
    for (int i = 0; i < mParticles.size(); i++)
    {
        mParticles[i].mass += 0.2;
    }
}

void SpringSystem::decreaseMasses()
{
    for (int i = 0; i < mParticles.size(); i++)
    {
        mParticles[i].mass -= 0.2;
        // limit values
        if (mParticles[i].mass <= 0.1)
        {
            mParticles[i].mass += 0.2;
        }
    }
}