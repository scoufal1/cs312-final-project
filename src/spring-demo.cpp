// Bryn Mawr College, alinen, 2020
//

#include "AGL.h"
#include "AGLM.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include "springsystem.h"
#include "renderer.h"

using namespace std;
using namespace glm;
using namespace agl;

SpringSystem theSystem;

float width = 500;
float height = 500;

bool mouse_pressed = false;
bool shift_pressed = false;
//bool movePart = false;
int selectedParticle = -1;

// used for camera movement
float lastXPos = 500.0f / 2;
float lastYPos = 500.0f / 2;
float azimuth = 0.0f;
float elevation = 0.0f;
float dist = 5.0f;

// used for moving particles
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
bool selected = false;
double lastx, lasty;
vec3 particlePos = vec3(0, 0, -2);
vec3 lastParticlePos = vec3(0, 0, 0);
float nearDistance = 0.1;
vec3 lookfrom(0, 0, dist);

bool pause = false; // pause and play motion

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == 'K')
    {
        theSystem.increaseKspring();
    }
    else if (key == 'J')
    {
        theSystem.decreaseKspring();
    }
    else if (key == 'M')
    {
        theSystem.increaseMasses();
    }
    else if (key == 'N')
    {
        theSystem.decreaseMasses();
    }
    else if (key == 'P')
    {
        pause = !pause;
    }
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Prevent a divide by zero
    if (height == 0)
        height = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, width, height);
    ParticleSystem::GetRenderer().perspective(radians(60.0f), 1.0f, 0.1f, 100.0f);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    double threshold = 20;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        mouse_pressed = true;

        // check if shift is pressed
        int keyPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        if (keyPress == GLFW_PRESS)
        {
            shift_pressed = true;
        }
        else
        {
            shift_pressed = false;
        }
        glfwGetCursorPos(window, &lastx, &lasty);

        // iterate through list of particles
        // if clicked point, set selected to true
        for (int i = 0; i < theSystem.mParticles.size(); i++)
        {
            particlePos = theSystem.mParticles[i].pos;

            // Convert the particle position to screen coords
            vec4 projectedPos = projectionMatrix * viewMatrix * vec4(particlePos, 1.0);
            vec3 ndcPos = vec3(projectedPos) / projectedPos.w;
            vec2 screenPos = (vec2(ndcPos) + vec2(1.0)) * 0.5f * vec2(width, height);

            // flip y coordinate because positive Y goes down instead of up
            screenPos.y = height - screenPos.y;

            if (length(screenPos - vec2(lastx, lasty)) < threshold)
            {
                selected = true;
                selectedParticle = i;
                lastParticlePos = particlePos;
            }
        }
    }

    if (action == GLFW_RELEASE)
    {
        selected = false;
        mouse_pressed = false;
    }
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (mouse_pressed && !selected)
    {

        // compute change in mouse movement
        float changeX = xpos - lastXPos;
        float changeY = ypos - lastYPos;

        // if shift pressed, change distance from object
        if (shift_pressed)
        {
            float s = 0.06f;
            dist += changeY * s;
            dist -= changeX * s;
        }
        else
        {
            azimuth -= changeX;
            elevation += changeY;

            // prevent object from flipping
            if (elevation > 85.0f)
                elevation = 85.0f;
            if (elevation < -85.0f)
                elevation = -85.0f;
        }
    }
    if (selected)
    {
        // move particle based on the change in position
        double dx = xpos - lastx;
        double dy = ypos - lasty;

        // convert movement into an amount to change the particle (in world space)
        vec2 screenDir(dx, -dy);
        vec2 ndcDir = (2.0f * screenDir / vec2(width, height));

        vec3 pDir = vec3(ndcDir, 0.0);
        mat4 PV = projectionMatrix * viewMatrix;
        vec3 wDir = vec3(inverse(PV) * vec4(pDir, 0.0));
        vec3 delta = wDir * length(lastParticlePos - lookfrom) / 1.0f;

        particlePos = lastParticlePos + delta;
        theSystem.mParticles[selectedParticle].pos = particlePos;
    }
    lastXPos = xpos;
    lastYPos = ypos;
}

int main(int argc, char **argv)
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        return -1;
    }

    // Explicitly ask for a 4.0 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Particle Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

#ifndef APPLE
    if (glewInit() != GLEW_OK)
    {
        cout << "Cannot initialize GLEW\n";
        return -1;
    }
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0, 0, 1);

    theSystem.init(15);
    float fov = radians(30.0f);

    ParticleSystem::GetRenderer().perspective(fov, 1.0f, 0.1f, 10.0f);
    ParticleSystem::GetRenderer().lookAt(vec3(0, 0, dist), vec3(0, 0, 0));

    float lastTime = glfwGetTime();

    double xpos, ypos;

    vec3 up = vec3(0.0, 1.0, 0.0);

    projectionMatrix = glm::perspective(radians(30.0f), 1.0f, nearDistance, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        viewMatrix = glm::lookAt(lookfrom, vec3(0), vec3(0, 1, 0));
        ParticleSystem::GetRenderer().setUp(vec3(0.0, 1.0, 0.0));

        // if not currently moving a particle, change camera position with mouse movement
        if (!selected)
        {
            // calculate lookfrom position
            float x = dist * sin(glm::radians(azimuth)) * cos(glm::radians(elevation));
            float y = dist * sin(glm::radians(elevation));
            float z = dist * cos(glm::radians(azimuth)) * cos(glm::radians(elevation));

            lookfrom = vec3(x, y, z);

            //calculate up vector
            vec3 forward = normalize(lookfrom);
            vec3 right = cross(normalize(vec3(0.0, 1.0, 0.0)), forward);
            up = cross(forward, right);
            ParticleSystem::GetRenderer().setUp(up);

            // update camera pos
            ParticleSystem::GetRenderer().lookAt(lookfrom, glm::vec3(0, 0, 0));
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

        float dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glfwGetCursorPos(window, &xpos, &ypos);

        // update forces and positions when mouse is released and system isn't paused
        if (!mouse_pressed)
        {
            if (!pause)
            {
                theSystem.calculateForces(dt);
                theSystem.update(dt);
            }
        }

        theSystem.draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
