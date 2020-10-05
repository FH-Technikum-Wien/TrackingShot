//
// Created by David on 10/4/2020.
//

#include "Spline.h"

glm::vec3 Spline::GetSplineTangent(glm::vec3 pm1, glm::vec3 p0, glm::vec3 p1) const
{
    return (1 - Tension) * (1 + Bias) * (1 + Continuity) * 0.5f * (p0 - pm1) +
            (1 - Tension) * (1 - Bias) * (1 - Continuity) * 0.5f * (p1 - p0);
}

glm::vec3 Spline::GetSplineTangentNextP(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) const
{
    return (1 - Tension) * (1 + Bias) * (1 - Continuity) * 0.5f * (p1 - p0) +
           (1 - Tension) * (1 - Bias) * (1 + Continuity) * 0.5f * (p2 - p1);
}

glm::vec3 Spline::Interpolate(glm::vec3 prev, glm::vec3 curr, glm::vec3 next, glm::vec3 nextNext, float t) const
{
    float t2 = t * t;
    float t3 = t2 * t;

    glm::vec3 tan0 = GetSplineTangent(prev, curr, next);
    glm::vec3 tan1 = GetSplineTangentNextP(curr, next, nextNext);

    return (2 * t3 - 3 * t2 + 1) * curr + (t3 - 2 * t2 + t) * tan0 + (-2 * t3 + 3 * t2) * next + (t3 - t2) * tan1;
}
