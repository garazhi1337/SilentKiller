#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(string tag, Model* model, Transform* transform)
{
	this->tag = tag;
	this->model = model;
	this->transform = transform;
}

GameObject::~GameObject()
{
	delete this;
}
