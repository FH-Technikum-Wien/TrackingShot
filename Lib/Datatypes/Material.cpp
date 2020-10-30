#include "Material.h"
#include "../Util.h"
#include "../Consts.h"
#include <GLFW/glfw3.h>

Material Material::ColorMat(glm::vec3 color)
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_WHITE, GL_RGB);
	return Material(&texture, color);
}

Material Material::GridMat(glm::vec3 color)
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_GRID, GL_RGB);
	return Material(&texture, color);
}

Material Material::WoodCrateMat(glm::vec3 color)
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_CRATE, GL_RGB);
	return Material(&texture, color);
}

Material Material::BrickMat(glm::vec3 color)
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_BRICK, GL_RGB);
	return Material(&texture, color);
}

Material Material::MetalMat()
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_WHITE, GL_RGB);
	Material material = Material(&texture, glm::vec3(0.2f, 0.2f, 0.2f));
	material.diffuseStrength = 0.2f;
	material.specularStrength = 1.0f;
	material.focus = 128;
	return material;
}

Material::Material()
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_WHITE, GL_RGB);
	this->texture = texture;
}

Material::Material(const unsigned int* texture)
{
	this->texture = *texture;
}

Material::Material(const unsigned int* texture, glm::vec3 color) : Material(texture)
{
	this->color = color;
}
