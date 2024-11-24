#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}


void Material::setAmbient(glm::vec3 ambient)
{
	this->ambient = ambient;
}

void Material::setDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void Material::setSpecular(glm::vec3 specular)
{
	this->specular = specular;
}

void Material::setShininess(float shininess)
{
	this->shininess = shininess;
}

