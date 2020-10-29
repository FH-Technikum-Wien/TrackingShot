#pragma once
#include <glm/vec3.hpp>

class Material
{
public:
	unsigned int texture = 0;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 1;
	float diffuseStrength = 1;
	float specularStrength = 0;
	float focus = 64;
	
	static Material ColorMat(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	static Material GridMat(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	static Material WoodCrateMat(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	static Material BrickMat(glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

	Material();
	Material(const unsigned int* texture);
	Material(const unsigned int* texture, glm::vec3 color);
};

