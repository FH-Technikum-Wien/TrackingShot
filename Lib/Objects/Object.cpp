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

	// Add normalMap
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, material.normalMap);

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
	vertexCount = arraySize / 8;
	// Generate Vertex-Array-Cube to store vertex attribute configuration and which VBO(s) to use
	glGenVertexArrays(1, &VAO);
	// Bind Vertex-Array-Cube to configure VBO(s)
	glBindVertexArray(VAO);


	// Generate Vertex-Buffer-Cube to manage memory on GPU and store vertices
	glGenBuffers(1, &VBO);
	// Bind Vertex-Buffer-Cube to configure it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy vertex data into buffer's memory (into VBO which is bound to GL_ARRAY_BUFFER)
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(float), vertices, GL_STATIC_DRAW);
	// POSITION
	// Tell OpenGL how to interpret/read the vertex data (per vertex attribute, e.g. one vertex point)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// NORMALS
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// UVs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Get tangents
	float* tangents = calculateTangents(vertices, vertexCount);
	glGenBuffers(1, &VBO_TANGENTS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TANGENTS);
	glBufferData(GL_ARRAY_BUFFER, (vertexCount * 3) * sizeof(float), tangents, GL_STATIC_DRAW);

	// Tangents
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(3);
}

float* Object::calculateTangents(const float* vertices, unsigned int tangentCount)
{
	unsigned int tangentsSize = tangentCount * 3;
	float* tangents = new float[tangentsSize];
	unsigned int tangentIndex = 0;
	for (unsigned int i = 0; i < tangentCount; i++)
	{
		// Get vertices
		unsigned int vertex1 = i * 8;
		unsigned int vertex2 = i * 8 + 1;
		unsigned int vertex3 = i * 8 + 2;
		glm::vec3 pos1 = glm::vec3(vertices[vertex1], vertices[vertex1 + 1], vertices[vertex1 + 2]);
		glm::vec3 pos2 = glm::vec3(vertices[vertex2], vertices[vertex2 + 1], vertices[vertex2 + 2]);
		glm::vec3 pos3 = glm::vec3(vertices[vertex3], vertices[vertex3 + 1], vertices[vertex3 + 2]);

		// Calculate edges
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;

		// Get UV's (by adding texture and normal offset)
		glm::vec2 uv1 = glm::vec2(vertices[vertex1 + 6], vertices[vertex1 + 7]);
		glm::vec2 uv2 = glm::vec2(vertices[vertex2 + 6], vertices[vertex2 + 7]);
		glm::vec2 uv3 = glm::vec2(vertices[vertex3 + 6], vertices[vertex3 + 7]);

		// Calculate deltaUV's
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangents[tangentIndex] = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangents[tangentIndex + 1] = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangents[tangentIndex + 2] = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangentIndex += 3;
	}
	return tangents;
}
