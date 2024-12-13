#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Transform.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Model.h"

class Game
{
public:
	int run();
private:
	void onResize(GLFWwindow* window, int width, int height);
	void onMouse(GLFWwindow* window, double posx, double posy);
	void processInput(GLFWwindow* window);
	float randFloat(float min, float max);
	glm::vec3 randSphericPosition(float x0, float y0, float z0, float minRadius, float maxRadius);
	const float WIDTH = 720.f;
	const float HEIGHT = 720.f;
	Camera* playerCamera;
};

