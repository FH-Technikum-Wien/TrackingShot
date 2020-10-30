#include "World.h"
#include <GLFW\glfw3.h>

void World::SetMovementSpeed(MovementSpeedLevel level)
{
	switch (level) {
	case MovementSpeedLevel::SLOW:
		camera.MovementSpeed = 0.5f;
		break;
	case MovementSpeedLevel::NORMAL:
		camera.MovementSpeed = 1.0f;
		break;
	case MovementSpeedLevel::FAST:
		camera.MovementSpeed = 2.0f;
		break;
	case MovementSpeedLevel::SONIC:
		camera.MovementSpeed = 10.0f;
		break;
	}
}

Camera& World::GetCamera()
{
	return camera;
}

void World::ToggleFreeFly()
{
	FreeFly = !FreeFly;
}

void World::AddPathPoint()
{
	if (isUsingDefaultPath) {
		isUsingDefaultPath = false;
		path = Path();
	}
	path.add(PathPoint(camera.Position, camera.Orientation));
}

void World::RemoveLastPathPoint()
{
	if(path.size() > 0)
		path.removeLast();
}

void World::ResetPath()
{
	isUsingDefaultPath = true;
	path = Path::DefaultPath();
}

void World::GoToNextPathPoint()
{
	path.setPathIndex(path.pathIndex + 1);
	GoToCurrentPathPoint();
}

void World::GoToPreviousPathPoint()
{
	path.setPathIndex(path.pathIndex - 1);
	GoToCurrentPathPoint();
}

void World::GoToCurrentPathPoint()
{
	path.applyCurrentPathPoint(camera);
}

void World::GoToFirstPathPoint()
{
	path.setPathIndex(0);
	GoToCurrentPathPoint();
}

void World::addObject(Object* object)
{
	objects.push_back(object);
}

void World::update(Shader& shader)
{
	// Delta calculations
	auto currentFrameTime = (float)glfwGetTime();
	DeltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	// Move camera
	if (!FreeFly)
		path.updateCameraTransform(camera);


	// Activate program.
	shader.activate();
	shader.setMat4("viewMat", camera.GetViewMat());

	renderWorld(shader);
}

void World::renderWorld(Shader& shader)
{
	// Render objects.
	for (Object* object : objects) {
		object->render(shader);
	}
}
