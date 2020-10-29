#pragma once
#include <vector>

#include "PathPoint.h"
#include "../World/Camera.h"
#include "../Math/Spline.h"

class Path
{
public:
	int pathIndex;

	Path(std::vector<PathPoint> pathPoints);
	static Path DefaultPath();

	void updateCameraTransform(Camera& camera);
	void applyCurrentPathPoint(Camera& camera);
	void add(PathPoint pathPoint);
	void removeLast();

	void setPathIndex(int index);
	int size();

private:
	float pathSectionProgress;
	std::vector<PathPoint> pathPoints;
	Spline spline = {0,0,0};
};

