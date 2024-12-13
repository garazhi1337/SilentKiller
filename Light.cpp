#pragma once

#define DIRECTIONAL 1
#define PROJECTOR 2
#define POINT 3

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"


class Light
{
private:
	glm::vec3 dirLightPos;
	glm::vec3 pointLightPos;
	glm::vec3 projectorLightPos;
	Shader* shader;
	int mode;
public:
	Light(Shader* shader)
	{
		this->shader = shader;
		setLightMode(DIRECTIONAL);
	}

	~Light()
	{
		delete this;
	}

	void setDirLightPos(glm::vec3 dirLightPos)
	{
		this->dirLightPos = dirLightPos;
		shader->setVec3("dirLightPos", dirLightPos);
	}

	void setPointLightPos(glm::vec3 pointLightPos)
	{
		this->pointLightPos = pointLightPos;
		shader->setVec3("pointLightPos", pointLightPos);
	}

	void setProjectorLightPos(glm::vec3 projectorLightPos)
	{
		this->projectorLightPos = projectorLightPos;
		shader->setVec3("projectorLightPos", projectorLightPos);
	}

	void setLightMode(int mode)
	{
		this->mode = mode;
		shader->setInt("lightMode", mode);
	}

	void changeLightMode(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			setLightMode(DIRECTIONAL);
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			setLightMode(PROJECTOR);
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			setLightMode(POINT);
	}
};