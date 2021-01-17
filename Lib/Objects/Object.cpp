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
	glDeleteBuffers(1, &VBO_VERTICES);
	glDeleteBuffers(1, &VBO_NORMALS);
	glDeleteBuffers(1, &VBO_UVS);
	glDeleteBuffers(1, &VBO_TANGENTS);
}

void Object::init(float* vertices, float* normals, float* uvs, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount)
{
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;
	this->vertices = vertices;
	this->indices = indices;
	// Generate Vertex-Array-Cube to store vertex attribute configuration and which VBO(s) to use
	glGenVertexArrays(1, &VAO);
	// Bind Vertex-Array-Cube to configure VBO(s)
	glBindVertexArray(VAO);

	// Generate Vertex-Buffer-Cube to manage memory on GPU and store vertices
	glGenBuffers(1, &VBO_VERTICES);
	// Bind Vertex-Buffer-Cube to configure it
	glBindBuffer(GL_ARRAY_BUFFER, VBO_VERTICES);
	// Copy vertex data into buffer's memory (into VBO which is bound to GL_ARRAY_BUFFER)
	glBufferData(GL_ARRAY_BUFFER, (vertexCount * 3) * sizeof(float), vertices, GL_STATIC_DRAW);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);


	// POSITION
	// Tell OpenGL how to interpret/read the vertex data (per vertex attribute, e.g. one vertex point)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// NORMALS
	glGenBuffers(1, &VBO_NORMALS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_NORMALS);
	glBufferData(GL_ARRAY_BUFFER, (vertexCount * 3) * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	// UVs
	glGenBuffers(1, &VBO_UVS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVS);
	glBufferData(GL_ARRAY_BUFFER, (vertexCount * 2) * sizeof(float), uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	// TANGENTS
	float* tangents = calculateTangents(vertices, uvs, vertexCount);
	glGenBuffers(1, &VBO_TANGENTS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TANGENTS);
	glBufferData(GL_ARRAY_BUFFER, (vertexCount * 3) * sizeof(float), tangents, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
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

	glBindBuffer(GL_ARRAY_BUFFER, VBO_VERTICES);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
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

float* Object::getVerticesInWorldSpace()
{
	float* worldVertices = new float[vertexCount * 3];
	for (int i = 0; i < vertexCount; i++)
	{
		int index = i * 3;

		glm::vec3 localPos = glm::vec3(vertices[index], vertices[index + 1], vertices[index + 2]);
		glm::vec4 worldPos = Transform * glm::vec4(localPos, 1);
		worldVertices[index] = worldPos.x;
		worldVertices[index + 1] = worldPos.y;
		worldVertices[index + 2] = worldPos.z;
	}
	return worldVertices;
}



float* Object::calculateTangents(float* vertices, float* uvs, unsigned int vertexCount)
{
	float* tangents = new float[vertexCount * 3];
	unsigned int tangentIndex = 0;
	for (unsigned int i = 0; i < vertexCount / 3; i++)
	{
		// Get vertices
		unsigned int vertex1 = i * 9;
		unsigned int vertex2 = i * 9 + 3;
		unsigned int vertex3 = i * 9 + 6;
		glm::vec3 pos1 = glm::vec3(vertices[vertex1], vertices[vertex1 + 1], vertices[vertex1 + 2]);
		glm::vec3 pos2 = glm::vec3(vertices[vertex2], vertices[vertex2 + 1], vertices[vertex2 + 2]);
		glm::vec3 pos3 = glm::vec3(vertices[vertex3], vertices[vertex3 + 1], vertices[vertex3 + 2]);

		// Calculate edges
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;

		// Get UV's
		unsigned int uvIndex1 = i * 6;
		unsigned int uvIndex2 = i * 6 + 2;
		unsigned int uvIndex3 = i * 6 + 4;
		glm::vec2 uv1 = glm::vec2(uvs[uvIndex1], uvs[uvIndex1 + 1]);
		glm::vec2 uv2 = glm::vec2(uvs[uvIndex2], uvs[uvIndex2 + 1]);
		glm::vec2 uv3 = glm::vec2(uvs[uvIndex3], uvs[uvIndex3 + 1]);

		// Calculate deltaUV's
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		// Calculate x,y,z for the tangents
		float tangentsX = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		float tangentsY = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		float tangentsZ = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		// All tangents same for triangle.
		for (int i = 0; i < 3; i++)
		{
			tangents[tangentIndex] = tangentsX;
			tangents[tangentIndex + 1] = tangentsY;
			tangents[tangentIndex + 2] = tangentsZ;
			tangentIndex += 3;
		}
	}
	return tangents;
}
