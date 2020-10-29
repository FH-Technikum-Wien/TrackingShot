#ifndef TRACKINGSHOT_PLANE_H
#define TRACKINGSHOT_PLANE_H

#include "Object.h"

class Plane : public Object
{
public:
    Plane(const Material& material, glm::vec3 position, glm::vec3 eulerAngles) : Object(material, position, eulerAngles) {
        init(vertices, 48);
    }

private:
    const float vertices[48] {
             10.0f,  10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 20.0f,  0.0f,
             10.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 20.0f, 20.0f,
            -10.0f,  10.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
             10.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 20.0f, 20.0f,
            -10.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f, 20.0f,
            -10.0f,  10.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    };
};

#endif //TRACKINGSHOT_PLANE_H
