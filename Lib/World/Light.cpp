#include "Light.h"
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "../Consts.h"

Light::Light(glm::vec3 position, float intensity, glm::vec3 color)
{
	this->position = position;
	this->color = color;
	this->intensity = intensity;
}

glm::mat4 Light::activateLight(Shader& shader)
{
	shader.activate();
	float nearPlane = 0.001f, farPlane = 50.0f;
	glm::mat4 lightProjection;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

	glm::mat4 lightViewMat = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -10.0f, 0.0f));
	// Transforms world space to light space.
	glm::mat4 lightSpaceMat = lightProjection * lightViewMat;

	shader.setFloat("lightIntensity", intensity);
	shader.setVec3("lightColor", color);
	shader.setVec3("lightPos", position);
	shader.setMat4("lightSpaceMat", lightSpaceMat);

	return lightSpaceMat;
}
