#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Transform.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureLoader.h"
#include "Camera.h"

void onResize(GLFWwindow* window, int width, int height);
void onMouse(GLFWwindow* window, double posx, double posy);
void processInput(GLFWwindow* window);
const float WIDTH = 720.f;
const float HEIGHT = 720.f;
Camera* playerCamera;

int main()
{
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

	Shader* shader = new Shader("vert.glsl", "frag.glsl");
	Transform* cubeTransform = new Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.3f, 0.3f, 0.3f));
	Transform* lightTransform = new Transform(glm::vec3(1.f, 1.f, -1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f));

	TextureLoader* textureLoader = new TextureLoader();
	textureLoader->loadTexture("images\\woodenbox.jpg");

	playerCamera = new Camera(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), window, WIDTH, HEIGHT);

	
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
		glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		processInput(window);

		//создание видовой матрицы
		glm::mat4 view = glm::mat4(1.f);
		//создание видовой матрицы
		view = playerCamera->rotate();
		playerCamera->move(window);
		//создание перспективной матрицы
		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.0f), WIDTH/HEIGHT, 0.01f, 100.0f);

		//1 куб
		shader->useProgram();


		glBindVertexArray(VAO);
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, cubeTransform->getPosition());
		model = glm::scale(model, cubeTransform->getScale());
		glm::mat4 pvm = projection * view * model;
		shader->setFloatMat4("pvm", pvm);
		shader->setVec3("color", glm::vec3(0.5f, 0.f, 0.f));
		shader->setVec3("lightPos", lightTransform->getPosition());
		shader->setVec3("lightColor", glm::vec3(1.f, 1.f, 1.f));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(lightVAO);
		glm::mat4 model1 = glm::mat4(1.f);
		model1 = glm::translate(model1, lightTransform->getPosition());
		model1 = glm::scale(model1, lightTransform->getScale());
		shader->setVec3("lightPos", glm::vec3(0.f, 0.f, 0.f));
		shader->setVec3("lightColor", glm::vec3(1.f, 1.f, 1.f));
		glm::mat4 pvm1 = projection * view * model1;
		shader->setFloatMat4("pvm", pvm1);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
