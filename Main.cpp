#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Transform.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "DirectionalLight.h"

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
	for (int i = 0; i < 25; i++)
	{
		float size = randFloat(0.3f, 1.1f);
		glm::vec3 rotation = glm::vec3(randFloat(0.f, 360.f), randFloat(0.f, 360.f), randFloat(0.f, 360.f));
		Transform* cubeTransform = new Transform(
			glm::vec3(randSphericPosition(0.f, 0.f, 0.f, 3.f, 4.f)),
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
	/*
	Transform* lightTransform = new Transform(
		glm::vec3(0.f, 0.0f, 0.f), 
		glm::vec3(0.f, 0.f, 0.f), 
		glm::vec3(0.1f, 0.1f, 0.1f), 
		glm::vec3(0.f, 0.f, 0.f)
	);
	*/

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data1 = stbi_load("images\\container.png", &width, &height, &channels, 0);
	stbi_set_flip_vertically_on_load(true);
	//std::cout << *data << std::endl;
	if (data1)
	{
		//сделать в классе шейдер в униформу текстуры потом
		if (channels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		}
	}
	else
	{
		std::cout << "Failed to load texture *quq quq*" << std::endl;
	}

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char* data2 = stbi_load("images\\steelpart.png", &width, &height, &channels, 0);
	//std::cout << *data << std::endl;
	if (data2)
	{
		if (channels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		}
	}
	else
	{
		std::cout << "Failed to load texture *quq quq*" << std::endl;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	stbi_image_free(data1);
	stbi_image_free(data2);

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

	/*	
	Light* light = new Light(
	glm::vec3(0.f, 0.f, 0.f),
	glm::vec3(1.f, 1.f, 1.f),
	glm::vec3(1.f, 1.f, 1.f),
	glm::vec3(1.f, 1.f, 1.f)
	);
	*/

	DirectionalLight* light = new DirectionalLight(
		glm::vec3(-0.3f, -0.2f, -0.1f),
		glm::vec3(1.f, 1.f, 1.f),
		glm::vec3(1.f, 1.f, 1.f),
		glm::vec3(1.f, 1.f, 1.f)
	);

	std::cout << light->getAmbient().x << std::endl;
	std::cout << light->getDiffuse().x << std::endl;
	std::cout << light->getSpecular().x << std::endl;
	std::cout << light->getDirection().x << std::endl;

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rows * (3 + 3 + 2), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	

	unsigned int lightVBO;
	glGenBuffers(1, &lightVBO);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* rows* (3 + 3 + 2), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.075f, 0.15f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		processInput(window);

		//создание видовой матрицы
		glm::mat4 view = glm::mat4(1.f);
		//создание видовой матрицы
		view = playerCamera->rotate();
		playerCamera->move(window);
		//создание перспективного вида
		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.0f), WIDTH/HEIGHT, 0.01f, 100.0f);

		//тут будут осуществляться все действия с освещаемым объектом
		glBindVertexArray(VAO);
		materialShader->useProgram();
		glm::mat4 model = glm::mat4(1.f);
		//model = glm::translate(model, cubeTransform->getPosition());
		//model = glm::scale(model, cubeTransform->getScale());
		//model = glm::rotate(model, -(float)glfwGetTime() * 0.5f, glm::vec3(0.f, 0.f, 1.f));

		materialShader->setVec3("cameraPos", playerCamera->getPos());

		materialShader->setVec3("material.ambient", standard->getAmbient());
		materialShader->setVec3("material.diffuse", standard->getDiffuse());
		materialShader->setVec3("material.specular", standard->getSpecular());
		materialShader->setFloat("material.shininess", standard->getShininess());

		materialShader->setVec3("light.ambient", light->getAmbient());
		materialShader->setVec3("light.diffuse", light->getDiffuse());
		materialShader->setVec3("light.specular", light->getSpecular());
		materialShader->setVec3("light.direction", light->getDirection());

		for (auto box : boxes)
		{
			model = glm::mat4(1.f);
			model = glm::translate(model, box->getPosition());
			model = glm::scale(model, box->getScale());
			model = glm::rotate(model, glm::radians(box->getAngles().x), glm::vec3(1.f, 0.f, 0.f));
			model = glm::rotate(model, glm::radians(box->getAngles().y), glm::vec3(0.f, 1.f, 0.f));
			model = glm::rotate(model, glm::radians(box->getAngles().z), glm::vec3(1.f, 0.f, 1.f));
			materialShader->setFloatMat4("m", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		materialShader->setFloatMat4("p", projection);
		materialShader->setFloatMat4("v", view);
		//тут будут осуществляться все действия с источником света
		/*
		glBindVertexArray(lightVAO);
		shader->useProgram();
		model = glm::mat4(1.f);
		lightTransform->setPosition(glm::vec3(sin(glfwGetTime() * 5) * 10, lightTransform->getPosition().y, cos(glfwGetTime() * 5) * 10));
		model = glm::translate(model, lightTransform->getPosition());
		model = glm::scale(model, lightTransform->getScale());
		shader->setVec3("color", light->getAmbient());
		shader->setFloatMat4("p1", projection);
		shader->setFloatMat4("v1", view);
		shader->setFloatMat4("m1", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/

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
