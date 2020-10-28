#include "Squad.h"

glm::quat Squad::Interpolate(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q3, float t)
{
	glm::quat conjugate1 = glm::conjugate(q1);
	glm::quat conjugate2 = glm::conjugate(q2);
	glm::quat a = q1 * glm::exp(-0.25f * (glm::log(conjugate1 * q2) + glm::log(conjugate1 * q0)));
	glm::quat b = q2 * glm::exp(-0.25f * (glm::log(conjugate2 * q3) + glm::log(conjugate2 * q1)));
	return glm::squad(q1, q2, a, b, t);
}
