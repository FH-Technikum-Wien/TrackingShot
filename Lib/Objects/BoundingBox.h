#pragma once

#include <glm/matrix.hpp>
#include "../../Shaders/Shader.h"

class BoundingBox
{
public:
	BoundingBox(glm::vec3 position, glm::vec3 max, glm::vec3 min);

	void render(const Shader& shader);

private:
	glm::mat4 Transform = glm::mat4(1.0f);

	unsigned int VAO = 0;
	unsigned int VBO_VERTICES = 0;
};

