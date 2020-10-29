#include "Object.h"
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>

Object::Object(const Material& material, glm::vec3 position, glm::vec3 eulerAngles)
{
    translate(position);
    rotate(eulerAngles);
    this->material = material;
}

Object::~Object()
{
    // De-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Object::render(const Shader& shader)
{
    // Add texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.texture);

    glBindVertexArray(VAO);
    shader.setMat4("modelMat", Transform);
    shader.setFloat("ambientStrength", material.ambientStrength);
    shader.setFloat("diffuseStrength", material.diffuseStrength);
    shader.setFloat("specularStrength", material.specularStrength);
    shader.setFloat("focus", material.focus);
    shader.setVec3("textureColor", material.color);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Object::translate(glm::vec3 translation)
{
    Transform = glm::translate(Transform, translation);
}

void Object::rotate(glm::vec3 eulerAngles)
{
    Transform = glm::rotate(Transform, glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    Transform = glm::rotate(Transform, glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    Transform = glm::rotate(Transform, glm::radians(eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Object::init(const float* vertices, int arraySize)
{
    {
        vertexCount = arraySize / 5;
        // Generate Vertex-Array-Cube to store vertex attribute configuration and which VBO(s) to use
        glGenVertexArrays(1, &VAO);

        // Generate Vertex-Buffer-Cube to manage memory on GPU and store vertices
        glGenBuffers(1, &VBO);

        // Bind Vertex-Array-Cube to configure VBO(s)
        glBindVertexArray(VAO);

        // Bind Vertex-Buffer-Cube to configure it
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Copy vertex data into buffer's memory (into VBO which is bound to GL_ARRAY_BUFFER)
        glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(float), vertices, GL_STATIC_DRAW);

        // POSITION
        // Tell OpenGL how to interpret/read the vertex data (per vertex attribute, e.g. one vertex point)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        // Vertex attributes are disabled by default
        glEnableVertexAttribArray(0);

        // TEXTURE
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // NORMALS
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}
