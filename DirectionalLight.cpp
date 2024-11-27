#include "DirectionalLight.h"
#include "Light.h"

void DirectionalLight::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}
