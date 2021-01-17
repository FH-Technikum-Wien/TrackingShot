#include "Line.h"

#include <glad\glad.h>

Line::Line(glm::vec3 start, glm::vec3 end)
{
	float vertices[6] {
		start.x, start.y, start.z,
		end.x, end.y, end.z,
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

void Line::render(const Shader& shader)
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

	glLineWidth(4);
	glDrawArrays(GL_LINES, 0, 4);
}
