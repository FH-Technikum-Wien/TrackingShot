#include "Point.h"

#include <glad\glad.h>

Point::Point(glm::vec3 position)
{
	float* vertices = new float[3]{
		position.x, position.y, position.z,
	};

	// Generate Vertex-Array-Cube to store vertex attribute configuration and which VBO(s) to use
	glGenVertexArrays(1, &VAO);
	// Bind Vertex-Array-Cube to configure VBO(s)
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_VERTICES);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_VERTICES);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Point::render(const Shader& shader)
{
	shader.setMat4("modelMat", Transform);
	shader.setVec3("textureColor", glm::vec3(1));
	glBindVertexArray(VAO);
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, 1);
}
