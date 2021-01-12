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

    float* getVerticesInWorldSpace();
    inline unsigned int getVertexCount() { return vertexCount; }

protected:
    unsigned int VAO = 0;
    unsigned int VBO_VERTICES = 0;
    unsigned int VBO_NORMALS = 0;
    unsigned int VBO_UVS = 0;
    unsigned int VBO_TANGENTS = 0;
    Material material;

    unsigned int vertexCount = 0;
    float* vertices = nullptr;

    void init(float* vertices, float* normals, float* uvs, int vertexCount);

private:
    float* calculateTangents(float* vertices, float* uvs, unsigned int vertexCount);
};


#endif //TRACKINGSHOT_OBJECT_H
