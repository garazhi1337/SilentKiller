#pragma once

#include <vector>
#include <string>
#include "Model.h"
#include "Transform.h"
#include "glm/glm.hpp"


class GameObject
{
public:
	GameObject();
	GameObject(string tag, Model* model, Transform* transform);
	~GameObject();
private:
	string tag;
	Model* model;
	Transform* transform;
};

