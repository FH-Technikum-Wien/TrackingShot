#include "Material.h"
#include "../Util.h"
#include "../Consts.h"
#include <GLFW/glfw3.h>

Material Material::BrickMat()
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_BRICK, GL_RGB);
	unsigned int normalMap = Util::LoadTexture(Consts::PATHS::NORMAL_MAP_BRICK, GL_RGB);
	Material material = Material(&texture, &normalMap, glm::vec3(1.0f));
	material.ambientStrength = 1;
	material.diffuseStrength = 1;
	material.specularStrength = 0;
	material.focus = 64;
	return material;
}

Material Material::WoodMat()
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_WOOD, GL_RGB);
	unsigned int normalMap = Util::LoadTexture(Consts::PATHS::NORMAL_MAP_WOOD, GL_RGB);
	Material material = Material(&texture, &normalMap, glm::vec3(1.0f));
	material.ambientStrength = 1;
	material.diffuseStrength = 1;
	material.specularStrength = 0.7f;
	material.focus = 8;
	return material;
}

Material Material::RocksMat()
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_ROCKS, GL_RGB);
	unsigned int normalMap = Util::LoadTexture(Consts::PATHS::NORMAL_MAP_ROCKS, GL_RGB);
	Material material = Material(&texture, &normalMap, glm::vec3(1.0f));
	material.ambientStrength = 1;
	material.diffuseStrength = 1;
	material.specularStrength = 0.3f;
	material.focus = 8;
	return material;
}

Material Material::Brick2Mat()
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_BRICK2, GL_RGB);
	unsigned int normalMap = Util::LoadTexture(Consts::PATHS::NORMAL_MAP_BRICK2, GL_RGB);
	Material material = Material(&texture, &normalMap, glm::vec3(1.0f));
	material.ambientStrength = 1;
	material.diffuseStrength = 1;
	material.specularStrength = 0.2f;
	material.focus = 32;
	return material;
}

Material Material::WhiteMat()
{
	unsigned int texture = Util::LoadTexture(Consts::PATHS::TEXTURE_WHITE, GL_RGB);
	unsigned int normalMap = Util::LoadTexture(Consts::PATHS::NORMAL_MAP_EMPTY, GL_RGB);
	Material material = Material(&texture, &normalMap, glm::vec3(1.0f));
	return material;
}

Material::Material(glm::vec3 color)
{
	this->texture = Util::LoadTexture(Consts::PATHS::TEXTURE_WHITE, GL_RGB);
	this->normalMap = Util::LoadTexture(Consts::PATHS::NORMAL_MAP_EMPTY, GL_RGB);
	this->color = color;
}

Material::Material(const unsigned int* texture)
{
	this->texture = *texture;
	this->normalMap = Util::LoadTexture(Consts::PATHS::NORMAL_MAP_EMPTY, GL_RGB);
}

Material::Material(const unsigned int* texture, glm::vec3 color) : Material(texture)
{
	this->color = color;
}

Material::Material(const unsigned int* texture, const unsigned int* normalMap, glm::vec3 color)
{
	this->texture = *texture;
	this->normalMap = *normalMap;
	this->color = color;
}
