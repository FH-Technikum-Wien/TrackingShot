//
// Created by David on 10/4/2020.
//

#ifndef TRACKINGSHOT_PATHPOINT_H
#define TRACKINGSHOT_PATHPOINT_H


#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct PathPoint
{
public:
    glm::vec3 Position;
    glm::quat Orientation;

    PathPoint(float x, float y,float z)
    {
        Position = glm::vec3 (x,y,z);
    }
};


#endif //TRACKINGSHOT_PATHPOINT_H
