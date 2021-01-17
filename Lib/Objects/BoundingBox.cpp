#include "BoundingBox.h"

#include <glad\glad.h>

BoundingBox::BoundingBox(glm::vec3 position, glm::vec3 max, glm::vec3 min)
{
	float vertices[72] {
		// Front
		min[0], min[1], max[2],
		max[0], min[1], max[2],

		max[0], min[1], max[2],
		max[0], max[1], max[2],

		max[0], max[1], max[2],
		min[0], max[1], max[2],

		min[0], max[1], max[2],
		min[0], min[1], max[2],

		// Back
		min[0], min[1], min[2],
		max[0], min[1], min[2],

		max[0], min[1], min[2],
		max[0], max[1], min[2],

		max[0], max[1], min[2],
		min[0], max[1], min[2],

		min[0], max[1], min[2],
		min[0], min[1], min[2],

		// Connect front and back
		min[0], min[1], max[2],
		min[0], min[1], min[2],

		max[0], min[1], max[2],
		max[0], min[1], min[2],

		max[0], max[1], max[2],
		max[0], max[1], min[2],

		min[0], max[1], max[2],
		min[0], max[1], min[2],
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

void BoundingBox::render(const Shader& shader)
{
	shader.setMat4("modelMat", Transform);
	shader.setVec3("textureColor", glm::vec3(1));
	glBindVertexArray(VAO);
	glLineWidth(2);
	glDrawArrays(GL_LINES, 0, 24);
}
