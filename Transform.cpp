#include "Transform.h"

glm::vec3 Transform::getPosition()
{
	return position;
}

glm::vec3 Transform::getRotationAxis()
{
	return rotationAxis;
}

glm::vec3 Transform::getScale()
{
	return scale;
}

glm::vec3 Transform::getRotationAngles()
{
	return rotationAngles;
}

void Transform::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Transform::setRotationAxis(glm::vec3 rotation)
{
	this->rotationAxis = rotation;
}

void Transform::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

void Transform::setRotationAngles(glm::vec3 rotationAngles)
{
	this->rotationAngles = rotationAngles;
}

Transform::Transform(glm::vec3 position, glm::vec3 rotationAxis, glm::vec3 rotationAngles, glm::vec3 scale)
{
	this->position = position;
	this->rotationAxis = rotationAxis;
	this->scale = scale;
	this->rotationAngles = rotationAngles;
}

Transform::Transform()
{
}

Transform::~Transform()
{
	delete this;
}
