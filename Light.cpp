#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Light.h"


void Light::setPosition(glm::vec3 position)
{
	this->position = position;
}

glm::vec3 Light::getPosition()
{
	return position;
}

void Light::setAmbient(glm::vec3 ambient)
{
	this->ambient = ambient;
}

glm::vec3 Light::getAmbient()
{
	return ambient;
}

void Light::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

glm::vec3 Light::getDiffuse()
{
	return diffuse;
}

void Light::setSpecular(glm::vec3 specular)
{
	this->specular = specular;
}

glm::vec3 Light::getSpecular()
{
	return specular;
}

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}
