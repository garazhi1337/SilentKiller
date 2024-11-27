#include "Transform.h"

glm::vec3 Transform::getPosition()
{
	return position;
}

glm::vec3 Transform::getRotationAxis()
{
	return rotation;
}

glm::vec3 Transform::getScale()
{
	return scale;
}

glm::vec3 Transform::getAngles()
{
	return angles;
}

void Transform::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Transform::setRotationAxis(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Transform::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

void Transform::setAngles(glm::vec3 angles)
{
	this->angles = angles;
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 angles)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->angles = angles;
}
