#pragma once

#include "Model.h"
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();
	void draw(Shader* shader, Camera* playerCamera, float screenWidth, float screenHeight);
	void setModels(vector<Model*> models);
	vector<Model*> getModels();

private:
	vector<Model*> models;
};

