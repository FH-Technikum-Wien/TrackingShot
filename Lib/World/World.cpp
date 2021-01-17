#include "World.h"
#include <GLFW\glfw3.h>
#include <iostream>

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
		camera.MovementSpeed = 5.0f;
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
	if (path.size() > 0)
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

float World::GetBumpiness()
{
	return normalMapBumpiness;
}

void World::SetBumpiness(float newBumpiness)
{
	normalMapBumpiness = newBumpiness;
}

void World::addObject(Object* object)
{
	objects.push_back(object);
}

void World::addBoundingBox(BoundingBox* box)
{
	boundingBoxes.push_back(box);
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
	shader.setVec3("cameraPos", camera.Position);
	shader.setFloat("bumpiness", normalMapBumpiness);

	renderWorld(shader);

	// Render bounding boxes without shadows.
	if (showKDTree) {
		for (BoundingBox* box : boundingBoxes) {
			box->render(shader);
		}
	}

	if (rayLine != nullptr)
		rayLine->render(shader);

	// Render intersection
	if (intersectionTriangle != nullptr) {
		// Draw intersecntion point
		intersectionPoint->render(shader);
		// Draw triangle
		intersectionTriangle->render(shader);
	}
}

void World::renderWorld(Shader& shader)
{
	// Render objects.
	for (Object* object : objects) {
		object->render(shader);
	}
}

void World::addRay(Line* line) 
{
	rayLine = line;
}

void World::addIntersection(Point* intersectionPoint, TriangleObject* intersectionTriangle)
{
	this->intersectionPoint = intersectionPoint;
	this->intersectionTriangle = intersectionTriangle;
}

std::vector<float> World::getAllObjectVertices()
{
	std::vector<float> vertices;
	for (Object* object : objects)
	{
		int vertexCount = object->getVertexCount();
		float* objectVertices = object->getVerticesInWorldSpace();
		std::vector<float> verticesVector(objectVertices, objectVertices + vertexCount * 3);
		vertices.insert(vertices.end(), verticesVector.begin(), verticesVector.end());
	}
	return vertices;
}

std::vector<unsigned int> World::getAllObjectIndices()
{
	unsigned int currentIndex = 0;
	std::vector<unsigned int> indices;
	for (Object* object : objects)
	{
		int indexCount = object->getIndexCount();
		unsigned int* objectIndices = object->getIndices();
		for (unsigned int i = 0; i < indexCount; i++)
			objectIndices[i] = objectIndices[i] + currentIndex;
		currentIndex += indexCount;
		std::vector<unsigned int> indicesVector(objectIndices, objectIndices + indexCount);
		indices.insert(indices.end(), indicesVector.begin(), indicesVector.end());
	}
	return indices;
}
