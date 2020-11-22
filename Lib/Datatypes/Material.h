#pragma once
#include <glm/vec3.hpp>

class Material
{
public:
	unsigned int texture = 0;
	unsigned int normalMap = 0;
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 1;
	float diffuseStrength = 1;
	float specularStrength = 0;
	float focus = 64;
	
	static Material BrickMat();
	static Material WoodMat();
	static Material RocksMat();
	static Material Brick2Mat();

	Material() = default;
	Material(const unsigned int* texture);
	Material(const unsigned int* texture, glm::vec3 color);
	Material(const unsigned int* texture, const unsigned int* normalMap, glm::vec3 color);
};

