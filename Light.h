#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
private:
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	void setPosition(glm::vec3 position);

	glm::vec3 getPosition();

	void setAmbient(glm::vec3 ambient);

	glm::vec3 getAmbient();

	void setDiffuse(glm::vec3 diffuse);

	glm::vec3 getDiffuse();

	void setSpecular(glm::vec3 specular);

	glm::vec3 getSpecular();

	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};