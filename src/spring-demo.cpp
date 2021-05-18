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

bool mouse_pressed = false;
bool shift_pressed = false;
//bool movePart = false;
int selectedParticle = -1;

float lastXPos = 500.0f / 2;
float lastYPos = 500.0f / 2;
float azimuth = 0.0f;
float elevation = 0.0f;
float dist = 4.0f;

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
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS)
    {
        mouse_pressed = true;
        int keyPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        if (keyPress == GLFW_PRESS)
        {
            shift_pressed = true;
        }
        else
        {
            shift_pressed = false;
        }
    }
    else if (state == GLFW_RELEASE)
    {
        mouse_pressed = false;
    }
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (mouse_pressed && selectedParticle < 0)
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
    float width = 500;
    float height = 500;
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

    theSystem.init(10);
    float fov = radians(30.0f);

    ParticleSystem::GetRenderer().perspective(fov, 1.0f, 0.1f, 10.0f);
    ParticleSystem::GetRenderer().lookAt(vec3(0, 0, dist), vec3(0, 0, 0));

    float lastTime = glfwGetTime();

    double xpos, ypos;

    vec3 up = vec3(0.0, 1.0, 0.0);
    while (!glfwWindowShouldClose(window))
    {
        ParticleSystem::GetRenderer().setUp(vec3(0.0, 1.0, 0.0));
        //if (!movePart)
        if (selectedParticle < 0)
        {
            // calculate lookfrom position
            float x = dist * sin(glm::radians(azimuth)) * cos(glm::radians(elevation));
            float y = dist * sin(glm::radians(elevation));
            float z = dist * cos(glm::radians(azimuth)) * cos(glm::radians(elevation));

            vec3 lookfrom = vec3(x, y, z);

            //calculate up vector
            vec3 forward = normalize(lookfrom);
            vec3 right = cross(normalize(vec3(0.0, 1.0, 0.0)), forward);
            up = cross(forward, right);
            ParticleSystem::GetRenderer().setUp(up);

            ParticleSystem::GetRenderer().lookAt(lookfrom, glm::vec3(0, 0, 0));
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

        float dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        glfwGetCursorPos(window, &xpos, &ypos);

        if (mouse_pressed)
        {
            // convert from screen coordinates
            // if mouse is pressed on on particle, change particle position to mouse position until release
            //movePart = theSystem.moveParticle(2 * (xpos / width) - 1, 2 * (-ypos / height) + 1, azimuth, elevation);
            //movePart = theSystem.moveParticle(xpos, ypos);
            //selectedParticle = theSystem.hitParticle(xpos, ypos);
            selectedParticle = theSystem.hitParticle(xpos, ypos);
            if (selectedParticle >= 0)
            {
                theSystem.moveParticle(selectedParticle, xpos, ypos);
            }
        }
        else
        {
            theSystem.calculateForces(dt);
            theSystem.update(dt);
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
