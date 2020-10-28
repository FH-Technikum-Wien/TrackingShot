#pragma once

#include <glm/gtx/quaternion.hpp>

class Squad
{
public:
	static glm::quat Interpolate(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q3, float t);
};

