//
// Created by David on 10/3/2020.
//

#ifndef TRACKINGSHOT_CAMERA_H
#define TRACKINGSHOT_CAMERA_H

#include <glm/vec3.hpp>

#include <GLFW/glfw3.h>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <algorithm>
#include <glm/gtc/quaternion.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN

};

class Camera
{
public:
    glm::vec3 Position = glm::vec3(0.0f);
    glm::quat Orientation = glm::quat(1.0f,0.0f,0.0f,0.0f);

    bool IsWindowFocused = false;

    explicit Camera(glm::vec3 position);

    glm::mat4 GetViewMat() const;

    void ProcessMouse(double xPos, double yPos);

    void ProcessInput(Camera_Movement movement, float deltaTime);

    void SetOrientation(glm::quat newRotation);

private:
    glm::mat4 transform = glm::mat4(1.0f);
    glm::vec2 lastMousePos = glm::vec2(0.0f);

    float xOffset;
    float yOffset;
};


#endif //TRACKINGSHOT_CAMERA_H
