#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "../../Shaders/Shader.h"

class Light
{
public:
	glm::vec3 position;
	glm::vec3 color;
	float intensity = 1;

	Light(glm::vec3 position, float intensity = 1, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 activateLight(Shader& shader);
};

