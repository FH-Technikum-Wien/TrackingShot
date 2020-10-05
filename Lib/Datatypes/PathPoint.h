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

    PathPoint(float x, float y,float z, float xRot = 0, float yRot = 0, float zRot = 0)
    {
        Position = glm::vec3 (x,y,z);
        Orientation = glm::quat(glm::vec3(xRot, yRot, zRot));
    }
};


#endif //TRACKINGSHOT_PATHPOINT_H
