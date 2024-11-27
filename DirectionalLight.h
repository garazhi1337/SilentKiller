#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 direction;
public:
	void setDirection(glm::vec3 direction);
	glm::vec3 getDirection();
	DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

