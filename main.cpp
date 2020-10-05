#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "stb-image/stb_image.h"
#include "Lib/Camera.h"
#include "Lib/Shader.h"
#include "Lib/Datatypes/PathPoint.h"
#include "Lib/Objects/Cube.h"
#include "Lib/Datatypes/Spline.h"
#include "Lib/Objects/Cube.h"
#include "Lib/Objects/Plane.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <list>

using namespace std;

unsigned int loadTexture(const char *path, unsigned int colorFormat);

void processInput(GLFWwindow *window);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void mousePositionCallback(GLFWwindow *window, double xPos, double yPos);

void windowFocusCallback(GLFWwindow *window, int isFocused);

void showFPS(GLFWwindow *window);

const unsigned int SCREEN_WIDTH = 1600;
const unsigned int SCREEN_HEIGHT = 1200;

const bool USE_WIREFRAME_MODE = false;

const char *vertexShaderPath = "shaders/shader.vert";
const char *fragmentShaderPath = "shaders/shader.frag";
const char *texturePath = "art/textures/woodenCrate.jpg";
const char *texturePath2 = "art/textures/brickWall.jpg";

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

// FPS calculation
int numberOfFrames = 0;
double lastTime = 0.0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

const float MOVEMENT_SPEED = 2.0f;

int main()
{
    //----------------------------------------------------------------------------------------------------
    // SETUP
    //----------------------------------------------------------------------------------------------------

    glfwInit();
    // Tell GLFW that we're using OpenGL version 3.3 .
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW to use Core_Profile -> Smaller subset without backwards-compatibility (not needed).
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create the GLFW-Window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Set OpenGL viewport.
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // Register resize method to resize-event.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (USE_WIREFRAME_MODE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //----------------------------------------------------------------------------------------------------
    // RENDERING
    //----------------------------------------------------------------------------------------------------

    Shader shader(vertexShaderPath, fragmentShaderPath);

    stbi_set_flip_vertically_on_load(true);
    unsigned int texture1 = loadTexture(texturePath, GL_RGB);
    unsigned int texture2 = loadTexture(texturePath2, GL_RGB);

    shader.activate();
    // Projection Matrix for adding perspective
    glm::mat4 projectionMat;
    projectionMat = glm::perspective(glm::radians(45.0f),
                                     (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projectionMat", projectionMat);

    Cube cube(&texture1, glm::vec3(), glm::vec3());
    Plane plane(&texture2, glm::vec3(0, -1,0), glm::vec3(90,0,0));

    //----------------------------------------------------------------------------------------------------
    // Path
    //----------------------------------------------------------------------------------------------------

    PathPoint path[] = {
            PathPoint(0.0f, 0.0f, -3.0f),
            PathPoint(2.0f, 0.0f, 0.0f),
            PathPoint(4.0f, 1.0f, -2.0f),
            PathPoint(2.0f, 1.0f, -4.0f),
            PathPoint(0.0f, 0.0f, -6.0f),
            PathPoint(-2.0f, 1.0f, -4.0f),
            PathPoint(-4.0f, 0.0f, -2.0f),
    };
    int pathCount = 7;

    float time = 0.0f;
    int index = 0;
    Spline spline;

    while (!glfwWindowShouldClose(window))
    {
        showFPS(window);
        // Delta time calculations
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Input
        processInput(window);

        // Sets one color for window (background)
        glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
        // Clear color buffer  nd depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Move camera
        float distance = glm::length((path[(index + 1) % pathCount].Position - path[index].Position));
        time += deltaTime;
        float t = (1 / distance) * time;
        t = min(1.0f, t);
        camera.Position = spline.Interpolate(path[index - 1].Position, path[index].Position,
                                             path[(index + 1) % pathCount].Position,
                                             path[(index + 2) % pathCount].Position, t);

        if (t == 1.0f)
        {
            time = 0.0f;
            index = (index + 1) % pathCount;
        }


        // Activate program
        shader.activate();
        shader.setMat4("viewMat", camera.GetViewMat());

        // render objects
        cube.render(shader);
        plane.render(shader);

        // Swaps the drawn buffer with the buffer that got written to.
        glfwSwapBuffers(window);
        // Checks if any events are triggered and executes callbacks.
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

unsigned int loadTexture(const char *path, unsigned int colorFormat)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping options. S == x-axis | T == y-axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // Set texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, numberOfChannels;
    unsigned char *data = stbi_load(path, &width, &height, &numberOfChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    return texture;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessInput(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessInput(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessInput(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessInput(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessInput(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessInput(DOWN, deltaTime);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mousePositionCallback(GLFWwindow *window, double xPos, double yPos)
{
    camera.ProcessMouse(xPos, yPos);
}

void windowFocusCallback(GLFWwindow *window, int isFocused)
{
    camera.IsWindowFocused = isFocused;
}

void showFPS(GLFWwindow *window)
{
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    numberOfFrames++;
    // Only update every second (or more)
    if (delta >= 1.0)
    {
        double fps = double(numberOfFrames) / delta;
        std::stringstream ss;
        ss << "TrackingShot - " << fps << " FPS]";
        glfwSetWindowTitle(window, ss.str().c_str());
        numberOfFrames = 0;
        lastTime = currentTime;
    }
}






