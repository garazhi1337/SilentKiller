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


void onResize(GLFWwindow* window, int width, int height);
void onMouse(GLFWwindow* window, double posx, double posy);
void processInput(GLFWwindow* window);
float randFloat(float min, float max);
glm::vec3 randSphericPosition(float x0, float y0, float z0, float minRadius, float maxRadius);
const float WIDTH = 720.f;
const float HEIGHT = 720.f;
Camera* playerCamera;

int main()
{
	std::srand(std::time(nullptr));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SilentKiller", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (window == NULL)
	{
		std::cout << "Failed to open GLFW window *quq quq*" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD *quq quq*" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	const int rows = 36;

	float vertices[rows * (3 + 3 + 2)] = {
		// координаты вершин	// нормали				// коорд. текстур
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
		 0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
		 0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,

		 0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		 0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f
	};

	std::srand(std::time(nullptr));
	std::vector<Transform*> boxes;
	for (int i = 0; i < 1000; i++)
	{
		float size = randFloat(0.3f, 1.1f);
		glm::vec3 rotation = glm::vec3(randFloat(0.f, 360.f), randFloat(0.f, 360.f), randFloat(0.f, 360.f));
		Transform* cubeTransform = new Transform(
			glm::vec3(randSphericPosition(0.f, 0.f, 0.f, 5.f, 10.f)),
			glm::vec3(1.f, 1.f, 1.f),
			glm::vec3(1.f, 1.f, 1.f) * size,
			rotation
		);
		boxes.push_back(cubeTransform);
	}

	Shader* shader = new Shader("vert.glsl", "frag.glsl");
	Shader* materialShader = new Shader("materialVert.glsl", "materialFrag.glsl");
	shader->useProgram();
	materialShader->useProgram();
	materialShader->setInt("material.diffuseMap", 0);
	materialShader->setInt("material.specularMap", 1);

	//Transform* cubeTransform = new Transform(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	Model* ball = new Model("models\\chair.obj");

	Transform* lightTransform = new Transform(
		glm::vec3(0.f, 0.0f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.1f, 0.1f, 0.1f), 
		glm::vec3(0.f, 0.f, 0.f)
	);

	playerCamera = new Camera(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), window, WIDTH, HEIGHT);

	Material* emerald = new Material(
		glm::vec3(0.0215f, 0.1745f, 0.0215f), 
		glm::vec3(0.07568f, 0.61424f, 0.07568f), 
		glm::vec3(0.633f, 0.727811f, 0.633f), 
		0.6f
	);

	Material* brass = new Material(
		glm::vec3(0.329412f, 0.223529f,	0.027451f),
		glm::vec3(0.780392f, 0.568627f,	0.113725f),
		glm::vec3(0.992157f, 0.941176f, 0.807843f),
		0.21794872f
	);

	Material* standard = new Material(
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(1.f, 1.f, 1.f),
		0.25f
	);

	DirectionalLight* light = new DirectionalLight(
		glm::vec3(-1.f, -1.f, -3.f),
		glm::vec3(1.f, 1.f, 1.f),
		glm::vec3(1.f, 1.f, 1.f),
		glm::vec3(1.f, 1.f, 1.f)
	);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.075f, 0.15f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		processInput(window);

		shader->useProgram();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = playerCamera->rotate();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(45.0f, (float)WIDTH/HEIGHT, 0.1f, 100.0f);
		shader->setFloatMat4("m", model);
		shader->setFloatMat4("v", view);
		shader->setFloatMat4("p", projection);
		ball->Draw(*shader);

		glfwSetWindowSizeCallback(window, onResize);
		glfwSetCursorPosCallback(window, onMouse);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void onResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void onMouse(GLFWwindow* window, double posx, double posy)
{
	playerCamera->onMouse(window, posx, posy);
}
void processInput(GLFWwindow* window)
{
	playerCamera->move(window);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float randFloat(float min, float max)
{
	float num = min + rand() * (max - min) / RAND_MAX;
	return num;
}

glm::vec3 randSphericPosition(float x0, float y0, float z0, float minRadius, float maxRadius)
{
	float x = randFloat(-maxRadius, maxRadius);
	float y = randFloat(-maxRadius, maxRadius);
	float z = randFloat(-maxRadius, maxRadius);
	float checkRadius = (float) sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	while ((checkRadius < minRadius || checkRadius > maxRadius))
	{
		x = randFloat(-maxRadius, maxRadius);
		y = randFloat(-maxRadius, maxRadius);
		z = randFloat(-maxRadius, maxRadius);
		checkRadius = (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	return glm::vec3(x + x0, y + y0, z + z0);
}
