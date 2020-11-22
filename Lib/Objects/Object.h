#ifndef TRACKINGSHOT_OBJECT_H
#define TRACKINGSHOT_OBJECT_H

#include <glm/matrix.hpp>
#include "../../Shaders/Shader.h"
#include "../Datatypes/Material.h"

class Object {
public:
    glm::mat4 Transform = glm::mat4(1.0f);

    Object(const Material& material, glm::vec3 position, glm::vec3 eulerAngles);

    ~Object();

    void render(const Shader& shader);

    void translate(glm::vec3 translation);

    void rotate(glm::vec3 eulerAngles);

protected:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int VBO_TANGENTS = 0;
    Material material;
    int vertexCount = 0;

    void init(const float* vertices, int arraySize);

private:
    float* calculateTangents(const float* vertices, unsigned int tangentCount);
};


#endif //TRACKINGSHOT_OBJECT_H
