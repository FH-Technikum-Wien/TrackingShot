#include "Input.h"
#include "Camera.h"
#include "World.h"

void Input::ProcessContinuousInput(GLFWwindow* window)
{
    Camera& camera = World::GetCamera();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessInput(CameraMovement::FORWARD);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessInput(CameraMovement::BACKWARD);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessInput(CameraMovement::LEFT);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessInput(CameraMovement::RIGHT);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessInput(CameraMovement::UP);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessInput(CameraMovement::DOWN);
}

void Input::ProcessSingleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        World::ToggleFreeFly();

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        World::SetMovementSpeed(MovementSpeedLevel::SLOW);

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        World::SetMovementSpeed(MovementSpeedLevel::NORMAL);

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        World::SetMovementSpeed(MovementSpeedLevel::FAST);

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        World::SetMovementSpeed(MovementSpeedLevel::SONIC);

    // Path control

    // Adds a path point to the custom path
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        World::AddPathPoint();
    }

    // Removes the last added path point from the custom path
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        World::RemoveLastPathPoint();
    }

    // Removes all custom path points and switches to default path
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        World::ResetPath();
    }

    // Move to first path point
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        World::GoToFirstPathPoint();
    }

    // Move to next path point
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        World::GoToNextPathPoint();
    }

    // Move to previous path point
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        World::GoToPreviousPathPoint();
    }

    // Focus current path point
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        World::GoToCurrentPathPoint();
    }
}

void Input::ProcessScrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
    float stepsSize = World::GetBumpiness() <= 1 ? 0.1f : 0.5f;
    float newBumpiness = (yoffset > 0 ? stepsSize : -stepsSize) + World::GetBumpiness();
    newBumpiness = std::min(10.0f, std::max(0.0f, newBumpiness));
    World::SetBumpiness(newBumpiness);
}
