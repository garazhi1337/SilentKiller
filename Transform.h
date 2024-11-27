#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 angles;
public:
	glm::vec3 getPosition();
	glm::vec3 getRotationAxis();
	glm::vec3 getScale();
	glm::vec3 getAngles();
	void setPosition(glm::vec3 position);
	void setRotationAxis(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setAngles(glm::vec3 angles);
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 angles);
};

