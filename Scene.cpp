#include "Scene.h"
#include "Camera.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	delete this;
}

void Scene::draw(Shader* shader, Camera* playerCamera, float screenWidth, float screenHeight)
{
	for (Model* model : models) 
	{
		model->draw(shader, playerCamera, screenWidth, screenHeight);
	}
}

void Scene::setModels(vector<Model*> models)
{
	this->models = models;
}

vector<Model*> Scene::getModels()
{
	return models;
}
