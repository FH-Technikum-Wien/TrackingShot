#include "Light.h"
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "../Consts.h"

Light::Light(glm::vec3 position, bool perspective, float intensity, glm::vec3 color)
{
	this->position = position;
	this->color = color;
	this->intensity = intensity;
	this->perspective = perspective;
}

glm::mat4 Light::activateLight(Shader& shader)
{
	float nearPlane = 0.01f, farPlane = 100.0f;
	glm::mat4 lightProjection;
	if(perspective)
		lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)Consts::RENDERING::SHADOW_WITH / (GLfloat)Consts::RENDERING::SHADOW_HEIGHT, nearPlane, farPlane);
	else
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

	glm::mat4 lightViewMat = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	// Transforms world space to light space.
	glm::mat4 lightSpaceMat = lightProjection * lightViewMat;

	shader.setFloat("lightIntensity", intensity);
	shader.setVec3("lightColor", color);
	shader.setVec3("lightPos", position);
	shader.setMat4("lightSpaceMat", lightSpaceMat);

	return lightSpaceMat;
}
