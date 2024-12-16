#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
private:
	glm::vec3 position;
	glm::vec3 rotationAxis;
	glm::vec3 scale;
	glm::vec3 rotationAngles;
public:
	glm::vec3 getPosition();
	glm::vec3 getRotationAxis();
	glm::vec3 getScale();
	glm::vec3 getRotationAngles();
	void setPosition(glm::vec3 position);
	void setRotationAxis(glm::vec3 rotationAxis);
	void setScale(glm::vec3 scale);
	void setRotationAngles(glm::vec3 rotationAngles);
	Transform(glm::vec3 position, glm::vec3 rotationAxis, glm::vec3 rotationAngles, glm::vec3 scale);
	Transform();
	~Transform();
};

