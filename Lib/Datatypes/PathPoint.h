//
// Created by David on 10/4/2020.
//

#ifndef TRACKINGSHOT_PATHPOINT_H
#define TRACKINGSHOT_PATHPOINT_H


#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct PathPoint {
public:
    glm::vec3 Position;
    glm::quat Orientation;

    PathPoint() = default;

    PathPoint(float x, float y, float z, float xRot, float yRot, float zRot) {
        Position = glm::vec3(x, y, z);
        Orientation = glm::quat(glm::vec3(glm::radians(xRot), glm::radians(yRot), glm::radians(zRot)));
    }

    PathPoint(glm::vec3 position, glm::quat orientation) {
        Position = position;
        Orientation = orientation;
    }
};


#endif //TRACKINGSHOT_PATHPOINT_H
