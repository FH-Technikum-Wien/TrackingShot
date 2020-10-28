#include "Path.h"
#include "../World/World.h"
#include "Squad.h"

Path::Path(std::vector<PathPoint> pathPoints)
{
	this->pathPoints = pathPoints;
}

Path Path::DefaultPath()
{
	return Path(std::vector<PathPoint>{
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
			PathPoint(-3.0f, 2.0f, 0.0f, -15.0f, 90.0f, 0.0f),
			PathPoint(0.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f),

			PathPoint(0.0f, 2.0f, 0.0f, -45.0f, 0.0f, 0.0f),
			PathPoint(0.0f, 4.0f, 0.0f, -90.0f, 0.0f, 0.0f),
			PathPoint(0.0f, 6.0f, 0.0f, -45.0f, 0.0f, 0.0f),
			PathPoint(0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f),
			PathPoint(0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f),
			PathPoint(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f),
			PathPoint(0.0f, 1.0f, 7.0f, 0.0f, 0.0f, 0.0f),
			PathPoint(0.0f, 2.0f, 10.0f, 0.0f, 0.0f, 0.0f)
	});
}

void Path::updateCameraTransform(Camera& camera)
{
	PathPoint prev = pathPoints[(pathIndex - 1) % pathPoints.size()];
	PathPoint curr = pathPoints[pathIndex];
	PathPoint next = pathPoints[(pathIndex + 1) % pathPoints.size()];
	PathPoint nextNext = pathPoints[(pathIndex + 2) % pathPoints.size()];

	float distance = glm::length(next.Position - curr.Position);
	// Value between 0 and distance
	pathSectionProgress += camera.MovementSpeed * World::DeltaTime;
	float t = (1 / distance) * pathSectionProgress;
	t = std::min(1.0f, t);
	camera.Position = spline.Interpolate(prev.Position, curr.Position, next.Position, nextNext.Position, t);

	// Rotate camera
	//camera.Orientation = glm::slerp(curr.Orientation,next.Orientation, t);
	camera.Orientation = Squad::Interpolate(prev.Orientation, curr.Orientation, next.Orientation, nextNext.Orientation, t);


	if (t == 1.0f) {
		pathSectionProgress = 0.0f;
		pathIndex = (pathIndex + 1) % pathPoints.size();
	}
}

void Path::applyCurrentPathPoint(Camera& camera)
{
	camera.Position = pathPoints[pathIndex].Position;
	camera.Orientation = pathPoints[pathIndex].Orientation;
}



void Path::add(PathPoint pathPoint)
{
	pathPoints.push_back(pathPoint);
}

void Path::removeLast()
{
	pathPoints.pop_back();
}

void Path::setPathIndex(int index)
{
	pathSectionProgress = 0;
	pathIndex = index % size();
}

int Path::size()
{
	return pathPoints.size();
}
