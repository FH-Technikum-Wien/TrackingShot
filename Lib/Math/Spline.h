//
// Created by David on 10/4/2020.
//

#ifndef TRACKINGSHOT_SPLINE_H
#define TRACKINGSHOT_SPLINE_H


#include <glm/vec3.hpp>


/**
 * @see https://www.wikiwand.com/en/Kochanek%E2%80%93Bartels_spline
 * @see https://www.wikiwand.com/en/Cubic_Hermite_spline
 */
class Spline
{

public:

    Spline() : Spline(0, 0, 0)
    {}

    Spline(float tension, float bias, float continuity)
    {
        Tension = tension;
        Bias = bias;
        Continuity = continuity;
    }

    glm::vec3 GetSplineTangent(glm::vec3 pm1, glm::vec3 p0, glm::vec3 p1) const;

    glm::vec3 GetSplineTangentNextP(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) const;

    glm::vec3 Interpolate(glm::vec3 prev, glm::vec3 curr, glm::vec3 next, glm::vec3 nextNext, float t);

private:
    float Tension;
    float Bias;
    float Continuity;
};


#endif //TRACKINGSHOT_SPLINE_H
