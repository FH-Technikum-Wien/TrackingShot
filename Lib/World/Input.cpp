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

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        World::ToggleFreeFly();
        std::string enabled = World::FreeFly ? "enabled" : "disabled";
        lastInput = "Free Fly " + enabled;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        World::SetMovementSpeed(MovementSpeedLevel::SLOW);
        lastInput = "Speed set to slow";
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        World::SetMovementSpeed(MovementSpeedLevel::NORMAL);
        lastInput = "Speed set to normal";
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        World::SetMovementSpeed(MovementSpeedLevel::FAST);
        lastInput = "Speed set to fast";
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        World::SetMovementSpeed(MovementSpeedLevel::SONIC);
        lastInput = "Speed set to sonic";
    }

    // Path control

    // Adds a path point to the custom path
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        World::AddPathPoint();
        lastInput = "Added path point";
    }

    // Removes the last added path point from the custom path
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        World::RemoveLastPathPoint();
        lastInput = "Removed last path point";
    }

    // Removes all custom path points and switches to default path
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        World::ResetPath();
        lastInput = "Reset path";
    }

    // Move to first path point
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        World::GoToFirstPathPoint();
        lastInput = "Go to first path point";
    }

    // Move to next path point
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        World::GoToNextPathPoint();
        lastInput = "Go to next path point";
    }

    // Move to previous path point
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        World::GoToPreviousPathPoint();
        lastInput = "Go to previous path point";
    }

    // Focus current path point
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        World::GoToCurrentPathPoint();
        lastInput = "Go to current path point";
    }

    // Antialiasing input
    if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
    {
        World::useAntiAliasing = !World::useAntiAliasing;
        std::string enabled = World::useAntiAliasing ? "enabled" : "disabled";
        lastInput = "Anti aliasing " + enabled;
    }
    // Show kdtree wireframe
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        World::showKDTree = !World::showKDTree;
        std::string enabled = World::showKDTree ? "enabled" : "disabled";
        lastInput = "Kd-Tree-Visual " + enabled;
    }
}

void Input::ProcessScrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
    float stepsSize = World::GetBumpiness() <= 1 ? 0.1f : 0.5f;
    float newBumpiness = (yoffset > 0 ? stepsSize : -stepsSize) + World::GetBumpiness();
    newBumpiness = std::min(10.0f, std::max(0.0f, newBumpiness));
    World::SetBumpiness(newBumpiness);
    lastInput = "Set bumpiness to " + std::to_string(newBumpiness);
}
