//
// Created by David on 10/3/2020.
//

#include <iostream>
#include "Camera.h"
#include "Consts.h"

Camera::Camera(glm::vec3 position)
{
    Position = position;
}

glm::mat4 Camera::GetViewMat() {
    return glm::inverse(glm::translate(transform, Position) * glm::mat4_cast(Orientation));
}

void Camera::ProcessMouse(double xPos, double yPos) {
    if (!IsWindowFocused) {
        lastMousePos = glm::vec2(xPos, yPos);
        IsWindowFocused = true;
    }

    if (!Consts::PLAY_MODE::FREE_FLY)
        return;

    xOffset += (lastMousePos.x - (float) xPos) * Consts::PLAYER::MOUSE_SENSITIVITY;
    yOffset += (lastMousePos.y - (float) yPos) * Consts::PLAYER::MOUSE_SENSITIVITY;
    lastMousePos = glm::vec2(xPos, yPos);

    Orientation = glm::quat(glm::vec3(yOffset, xOffset, 0.0f));
}

void Camera::ProcessInput(Camera_Movement movement, float deltaTime)
{
    const float cameraSpeed = Consts::PLAYER::CAMERA_SPEED * deltaTime;

    switch (movement)
    {
        case FORWARD:
            Position += Orientation * glm::vec3(0.0f, 0.0f, -1.0f) * cameraSpeed;
            break;
        case BACKWARD:
            Position -= Orientation * glm::vec3(0.0f, 0.0f, -1.0f) * cameraSpeed;
            break;
        case LEFT:
            Position -= Orientation * glm::vec3(1.0f, 0.0f, 0.0f) * cameraSpeed;
            break;
        case RIGHT:
            Position += Orientation * glm::vec3(1.0f, 0.0f, 0.0f) * cameraSpeed;
            break;
        case UP:
            Position += Orientation * glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed;
            break;
        case DOWN:
            Position -= Orientation * glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed;
            break;
    }
}
