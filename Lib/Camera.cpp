//
// Created by David on 10/3/2020.
//

#include <iostream>
#include "Camera.h"

const float MOVEMENT_SPEED = 2.5f;
const float MOUSE_SENSITIVITY = 0.001f;

Camera::Camera(glm::vec3 position)
{
    Position = position;
}

glm::mat4 Camera::GetViewMat() const
{
    return glm::inverse(glm::translate(transform, Position) * glm::mat4_cast(Orientation));
}

void Camera::ProcessMouse(double xPos, double yPos)
{
    if (!IsWindowFocused)
    {
        lastMousePos = glm::vec2(xPos, yPos);
        IsWindowFocused = true;
    }

    xOffset += (lastMousePos.x - (float) xPos) * MOUSE_SENSITIVITY;
    yOffset += (lastMousePos.y - (float) yPos) * MOUSE_SENSITIVITY;
    lastMousePos = glm::vec2(xPos, yPos);

    Orientation = glm::quat(glm::vec3(yOffset, xOffset, 0.0f));
}

void Camera::ProcessInput(Camera_Movement movement, float deltaTime)
{
    const float movementSpeed = MOVEMENT_SPEED * deltaTime;

    switch (movement)
    {
        case FORWARD:
            Position += Orientation * glm::vec3(0.0f, 0.0f, -1.0f) * movementSpeed;
            break;
        case BACKWARD:
            Position -= Orientation * glm::vec3(0.0f, 0.0f, -1.0f) * movementSpeed;
            break;
        case LEFT:
            Position -= Orientation * glm::vec3(1.0f, 0.0f, 0.0f) * movementSpeed;
            break;
        case RIGHT:
            Position += Orientation * glm::vec3(1.0f, 0.0f, 0.0f) * movementSpeed;
            break;
        case UP:
            Position += Orientation * glm::vec3(0.0f, 1.0f, 0.0f) * movementSpeed;
            break;
        case DOWN:
            Position -= Orientation * glm::vec3(0.0f, 1.0f, 0.0f) * movementSpeed;
            break;
    }
}

void Camera::SetOrientation(glm::quat newRotation)
{
    Orientation = newRotation;
}
