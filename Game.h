#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "Model.h"

static void handleOnResize(GLFWwindow* window, int width, int height);
static void handleOnMouse(GLFWwindow* window, double xpos, double ypos);

class Game
{
public:
	GLFWwindow* window;
	Game();
	~Game();
	int run();
	void onResize(GLFWwindow* window, int width, int height);
	void onMouse(GLFWwindow* window, double posx, double posy);

private:
	void processInput(GLFWwindow* window);
	float randFloat(float min, float max);
	glm::vec3 randSphericPosition(float x0, float y0, float z0, float minRadius, float maxRadius);
	const float WIDTH = 800.f;
	const float HEIGHT = 800.f;
	Camera* playerCamera;

};
