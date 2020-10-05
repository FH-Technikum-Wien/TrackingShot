//
// Created by David on 10/5/2020.
//

#ifndef TRACKINGSHOT_OBJECT_H
#define TRACKINGSHOT_OBJECT_H

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Shader.h"

class Object {
public:
    glm::mat4 Transform = glm::mat4(1.0f);

    Object(const unsigned int *texture, glm::vec3 position, glm::vec3 eulerAngles) {
        translate(position);
        rotate(eulerAngles);
        this->texture = *texture;
    }

    ~Object(){
        // De-allocate all resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void render(const Shader& shader) const{
        // Add texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        shader.setMat4("modelMat", Transform);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void translate(glm::vec3 translation){
        Transform = glm::translate(Transform, translation);
    }

    void rotate(glm::vec3 eulerAngles){
        Transform = glm::rotate(Transform, glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
        Transform = glm::rotate(Transform, glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
        Transform = glm::rotate(Transform, glm::radians(eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    }

protected:
    unsigned int VAO{};
    unsigned int VBO{};
    unsigned int texture{};
    int vertexCount{};

    void init(const float *vertices, int arraySize){
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        // Vertex attributes are disabled by default
        glEnableVertexAttribArray(0);

        // TEXTURE
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
};


#endif //TRACKINGSHOT_OBJECT_H
