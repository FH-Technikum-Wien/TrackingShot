#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "stb-image/stb_image.h"
#include "Lib/Camera.h"
#include "Shaders/Shader.h"
#include "Lib/Datatypes/PathPoint.h"
#include "Lib/Objects/Cube.h"
#include "Lib/Datatypes/Spline.h"
#include "Lib/Objects/Cube.h"
#include "Lib/Objects/Plane.h"
#include "Lib/Consts.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <list>
#include <vector>

using namespace std;


unsigned int loadTexture(const char *path, unsigned int colorFormat);

void processContinuousInput(GLFWwindow *window);

void processSingleInput(GLFWwindow *window, int key, int scancode, int action, int mods);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void mousePositionCallback(GLFWwindow *window, double xPos, double yPos);

void windowFocusCallback(GLFWwindow *window, int isFocused);

void showUI(GLFWwindow *window);

enum MovementSpeedLevel {
    SLOW,
    NORMAL,
    FAST,
    SONIC
};

void setMovementSpeed(MovementSpeedLevel level);

stringstream lastAction;

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

// FPS calculation
int numberOfFrames = 0;
double lastTime = 0.0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

list<Object *> worldObjects;

vector<PathPoint> path;
vector<PathPoint> customPath;
int pathIndex = 0;
float pathSectionProgress = 0.0f;
bool usingCustomPath;

vector<PathPoint> defaultPath = {
        PathPoint(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f),
        PathPoint(3.0f, 0.0f, 0.0f, 0.0f, 90.0f, 0.0f),
        PathPoint(0.0f, 0.0f, -3.0f, 0.0f, 180.0f, 0.0f),
        PathPoint(-3.0f, 0.0f, 0.0f, 0.0f, 270.0f, 0.0f),
        PathPoint(-3.0f, 5.0f, 0.0f, -70.0f, 270.0f, 0.0f),

        PathPoint(-15.0f, 7.0f, 0.0f, -45.0f, 270.0f, 0.0f),
        PathPoint(0.0f, 7.0f, -15.0f, -30.0f, 180.0f, 0.0f),
        PathPoint(15.0f, 7.0f, 0.0f, -30.0f, 90.0f, 0.0f),
        PathPoint(0.0f, 5.0f, 15.0f, -30.0f, 0.0f, 0.0f),

        PathPoint(0.0f, 2.0f, 0.0f, -15.0f, 90.0f, 0.0f),
        PathPoint(0.0f, 2.0f, -3.0f, -15.0f, 180.0f, 0.0f),
        PathPoint(0.0f, 2.0f, 0.0f, -15.0f, 270.0f, 0.0f),
        PathPoint(3.0f, 2.0f, 0.0f, -15.0f, 270.0f, 0.0f),

        PathPoint(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        PathPoint(0.0f, 3.0f, -2.0f, 0.0f, 0.0f, 0.0f),
        PathPoint(0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        PathPoint(2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        PathPoint(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
        PathPoint(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f),
        PathPoint(0.0f, 1.0f, 10.0f, 0.0f, 0.0f, 0.0f)
};

int main() {
    //----------------------------------------------------------------------------------------------------
    // SETUP
    //----------------------------------------------------------------------------------------------------

    glfwInit();
    // Tell GLFW that we're using OpenGL version 3.3 .
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW to use Core_Profile -> Smaller subset without backwards-compatibility (not needed).
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Consts::SCREEN::WIDTH, Consts::SCREEN::HEIGHT, "", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create the GLFW-Window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Set OpenGL viewport.
    glViewport(0, 0, Consts::SCREEN::WIDTH, Consts::SCREEN::HEIGHT);
    // Register resize method to resize-event.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetWindowFocusCallback(window, windowFocusCallback);
    glfwSetKeyCallback(window, processSingleInput);

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    if (Consts::RENDERING::USE_WIREFRAME_MODE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //----------------------------------------------------------------------------------------------------
    // RENDERING
    //----------------------------------------------------------------------------------------------------

    Shader shader(Consts::PATHS::VERTEX_SHADER, Consts::PATHS::FRAGMENT_SHADER);

    stbi_set_flip_vertically_on_load(true);
    unsigned int texture1 = loadTexture(Consts::PATHS::TEXTURE_1, GL_RGB);
    unsigned int texture2 = loadTexture(Consts::PATHS::GRID_TEX_1, GL_RGB);
    unsigned int texture3 = loadTexture(Consts::PATHS::GRID_TEX_2, GL_RGB);

    shader.activate();
    // Projection Matrix for adding perspective
    glm::mat4 projectionMat;
    projectionMat = glm::perspective(glm::radians(45.0f),
                                     (float) Consts::SCREEN::WIDTH / (float) Consts::SCREEN::HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projectionMat", projectionMat);


    // Add objects
    worldObjects.push_back(new Cube(&texture1, glm::vec3(), glm::vec3()));
    worldObjects.push_back(new Cube(&texture1, glm::vec3(0.0f, 3.0f, -7.0f), glm::vec3()));
    worldObjects.push_back(new Cube(&texture1, glm::vec3(-3.0f, 0.0f, 2.0f), glm::vec3(45.0f, 45.0f, 0.0f)));
    worldObjects.push_back(new Cube(&texture1, glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(120.0f, 0.0f, 0.0f)));
    worldObjects.push_back(new Cube(&texture1, glm::vec3(2.0f, 1.0f, 4.0f), glm::vec3(70.0f, 120.0f, 45.0f)));
    worldObjects.push_back(new Plane(&texture2, glm::vec3(0, -1, 0), glm::vec3(90, 0, 0)));

    path = defaultPath;
    Spline spline;

    while (!glfwWindowShouldClose(window)) {
        showUI(window);
        // Delta progress calculations
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Input
        processContinuousInput(window);

        // Sets one color for window (background)
        glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
        // Clear color buffer  nd depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (!Consts::PLAY_MODE::FREE_FLY) {
            // Move camera

            float distance = glm::length((path[(pathIndex + 1) % path.size()].Position - path[pathIndex].Position));
            // Value between 0 and distance
            pathSectionProgress += Consts::PLAYER::MOVEMENT_SPEED * deltaTime;
            float t = (1 / distance) * pathSectionProgress;
            t = min(1.0f, t);
            camera.Position = spline.Interpolate(path[pathIndex - 1].Position,
                                                 path[pathIndex].Position,
                                                 path[(pathIndex + 1) % path.size()].Position,
                                                 path[(pathIndex + 2) % path.size()].Position, t);

            // Rotate camera
            camera.Orientation = glm::slerp(path[(pathIndex)].Orientation,
                                            path[(pathIndex + 1) % path.size()].Orientation, t);
            if (t == 1.0f) {
                pathSectionProgress = 0.0f;
                pathIndex = (pathIndex + 1) % path.size();
            }


        }

        // Activate program
        shader.activate();
        shader.setMat4("viewMat", camera.GetViewMat());

        // render objects
        for (Object *object : worldObjects)
            object->render(shader);

        // Swaps the drawn buffer with the buffer that got written to.
        glfwSwapBuffers(window);
        // Checks if any events are triggered and executes callbacks.
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

unsigned int loadTexture(const char *path, unsigned int colorFormat) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping options. S == x-axis | T == y-axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, numberOfChannels;
    unsigned char *data = stbi_load(path, &width, &height, &numberOfChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    return texture;
}

void processContinuousInput(GLFWwindow *window) {

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

void processSingleInput(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        Consts::PLAY_MODE::FREE_FLY = !Consts::PLAY_MODE::FREE_FLY;
        lastAction << "Free Fly = " << Consts::PLAY_MODE::FREE_FLY;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        setMovementSpeed(SLOW);

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        setMovementSpeed(NORMAL);

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        setMovementSpeed(FAST);

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        setMovementSpeed(SONIC);

    // Path control

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        customPath.emplace_back(camera.Position, camera.Orientation);
        lastAction << "Custom path node added";
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        if (!customPath.empty()) {
            customPath.pop_back();
            lastAction << "Custom path node deleted";
        }
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        path = customPath;
        usingCustomPath = true;
        lastAction << "Switched to custom path";
    }

    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        path = defaultPath;
        customPath.clear();
        usingCustomPath = false;
        lastAction << "Deleted custom path";
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        pathIndex = 0;
        pathSectionProgress = 0.0f;
        lastAction << "Path restarted";
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mousePositionCallback(GLFWwindow *window, double xPos, double yPos) {
    camera.ProcessMouse(xPos, yPos);
}

void windowFocusCallback(GLFWwindow *window, int isFocused) {
    camera.IsWindowFocused = isFocused;
}

void showUI(GLFWwindow *window) {
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    numberOfFrames++;
    // Only update every second (or more)
    if (delta >= 1.5) {
        double fps = double(numberOfFrames) / delta;
        std::stringstream ss;
        ss << "TrackingShot ";
        ss << " [UsingCustomPath=" << usingCustomPath << "]";
        ss << " [Custom path nodes: " << customPath.size() << "]";
        ss << " [Current node: " << pathIndex << "]";
        ss << " [Last Action: " << lastAction.str() << "]";
        ss << " [" << fps << " FPS]";
        glfwSetWindowTitle(window, ss.str().c_str());
        numberOfFrames = 0;
        lastTime = currentTime;
        lastAction.str("");
    }
}

void setMovementSpeed(MovementSpeedLevel level) {

    lastAction << "Speed changed to: " << level;
    switch (level) {
        case SLOW:
            Consts::PLAYER::MOVEMENT_SPEED = 0.5f;
            break;
        case NORMAL:
            Consts::PLAYER::MOVEMENT_SPEED = 1.0f;
            break;
        case FAST:
            Consts::PLAYER::MOVEMENT_SPEED = 2.0f;
            break;
        case SONIC:
            Consts::PLAYER::MOVEMENT_SPEED = 10.0f;
            break;
    }
}






