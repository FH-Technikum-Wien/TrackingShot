#ifndef TRACKINGSHOT_OBJECT_H
#define TRACKINGSHOT_OBJECT_H

#include <glm/matrix.hpp>
#include <string>
#include "../../Shaders/Shader.h"
#include "../Datatypes/Material.h"

class Object {
public:
    glm::mat4 Transform = glm::mat4(1.0f);

    Object(const Material& material, glm::vec3 position, glm::vec3 eulerAngles);

    ~Object();

    void init(float* vertices, float* normals, float* uvs, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);
    
    void render(const Shader& shader);

    void translate(glm::vec3 translation);

    void rotate(glm::vec3 eulerAngles);

    float* getVerticesInWorldSpace();
    inline unsigned int getVertexCount() { return vertexCount; }
    inline unsigned int* getIndices() { return indices; }
    inline unsigned int getIndexCount() { return indexCount; }

protected:
    unsigned int VAO = 0;
    unsigned int EBO = 0;
    unsigned int VBO_VERTICES = 0;
    unsigned int VBO_NORMALS = 0;
    unsigned int VBO_UVS = 0;
    unsigned int VBO_TANGENTS = 0;
    Material material;

    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
    float* vertices = nullptr;
    unsigned int* indices = nullptr;


private:
    float* calculateTangents(float* vertices, float* uvs, unsigned int vertexCount);
};


#endif //TRACKINGSHOT_OBJECT_H
