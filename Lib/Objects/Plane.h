//
// Created by David on 10/5/2020.
//

#ifndef TRACKINGSHOT_PLANE_H
#define TRACKINGSHOT_PLANE_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Shaders/Shader.h"



class Plane : public Object
{
public:
    Plane(const unsigned int *texture, glm::vec3 position, glm::vec3 eulerAngles) : Object(texture, position, eulerAngles) {
        init(vertices, 30);
    }

private:
    const float vertices[30] {
            10.0f,  10.0f, 0.0f, 10.0f, 0.0f,
            10.0f, -10.0f, 0.0f, 10.0f, 10.0f,
            -10.0f,  10.0f, 0.0f, 0.0f, 0.0f,
            10.0f, -10.0f, 0.0f, 10.0f, 10.0f,
            -10.0f, -10.0f, 0.0f, 0.0f, 10.0f,
            -10.0f,  10.0f, 0.0f, 0.0f, 0.0f,
    };
};

#endif //TRACKINGSHOT_PLANE_H
