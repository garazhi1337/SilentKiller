#include "Transform.h"

glm::vec3 Transform::getPosition()
{
	return position;
}

glm::vec3 Transform::getRotation()
{
	return rotation;
}

glm::vec3 Transform::getScale()
{
	return scale;
}

void Transform::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Transform::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Transform::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}
