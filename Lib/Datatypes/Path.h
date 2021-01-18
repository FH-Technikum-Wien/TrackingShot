#pragma once
#include <vector>

#include "PathPoint.h"
#include "../World/Camera.h"
#include "../Math/Spline.h"

class Path
{
public:
	int pathIndex = 0;

	Path() = default;
	Path(std::vector<PathPoint> pathPoints);
	static Path DefaultPath();

	void updateCameraTransform(Camera& camera);
	void applyCurrentPathPoint(Camera& camera);
	void add(PathPoint pathPoint);
	void removeLast();

	void setPathIndex(int index);
	int size();

private:
	float pathSectionProgress = 0.0f;
	std::vector<PathPoint> pathPoints = std::vector<PathPoint>();
	Spline spline = {0,0,0};
};

