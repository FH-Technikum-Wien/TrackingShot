#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include "World.h"

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

    if (!World::FreeFly)
        return;

    xOffset += (lastMousePos.x - (float) xPos) * MouseSensitivity;
    yOffset += (lastMousePos.y - (float) yPos) * MouseSensitivity;
    lastMousePos = glm::vec2(xPos, yPos);

    Orientation = glm::quat(glm::vec3(yOffset, xOffset, 0.0f));
}

void Camera::ProcessInput(CameraMovement movement)
{
    const float movementSpeed = MovementSpeed * World::DeltaTime;

    switch (movement)
    {
        case CameraMovement::FORWARD:
            Position += Orientation * glm::vec3(0.0f, 0.0f, -1.0f) * movementSpeed;
            break;
        case CameraMovement::BACKWARD:
            Position -= Orientation * glm::vec3(0.0f, 0.0f, -1.0f) * movementSpeed;
            break;
        case CameraMovement::LEFT:
            Position -= Orientation * glm::vec3(1.0f, 0.0f, 0.0f) * movementSpeed;
            break;
        case CameraMovement::RIGHT:
            Position += Orientation * glm::vec3(1.0f, 0.0f, 0.0f) * movementSpeed;
            break;
        case CameraMovement::UP:
            Position += Orientation * glm::vec3(0.0f, 1.0f, 0.0f) * movementSpeed;
            break;
        case CameraMovement::DOWN:
            Position -= Orientation * glm::vec3(0.0f, 1.0f, 0.0f) * movementSpeed;
            break;
    }
}
