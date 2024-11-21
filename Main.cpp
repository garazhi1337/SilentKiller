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

	const int rows = 8;

	float vertices[rows * (3+4+2)] = {
			//координаты			цвета						координаты текстур
/*0*/		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f,
/*1*/		 1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f,
/*2*/		 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 0.0f,
/*3*/		-1.0f, 1.0f, 1.0f,	    1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f,
/*4*/		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
/*5*/		 1.0f, -1.0f, -1.0f,	1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f,
/*6*/		 1.0f, -1.0f, 1.0f,		0.5f, 0.5f, 0.5f, 1.0f,		1.0f, 0.0f,
/*7*/		-1.0f, -1.0f, 1.0f,	    0.6f, 0.6f, 0.9f, 1.0f,		0.0f, 0.0f
	};

	unsigned int elements[] = {
		0, 1, 3,
		1, 2, 3, 
		
		0, 4, 1,
		1, 4, 5,

		0, 3, 7,
		0, 7, 4, 

		1, 6, 2,
		1, 5, 6,

		2, 7, 3,
		2, 6, 7,

		4, 7, 5,
		5, 7, 6
	};

	float lightVertices[rows * 3] = {
		//координаты			
		/*0*/		-1.0f, 1.0f, -1.0f,	
		/*1*/		 1.0f, 1.0f, -1.0f,	
		/*2*/		 1.0f, 1.0f, 1.0f,	
		/*3*/		-1.0f, 1.0f, 1.0f,	   
		/*4*/		-1.0f, -1.0f, -1.0f,
		/*5*/		 1.0f, -1.0f, -1.0f,
		/*6*/		 1.0f, -1.0f, 1.0f,	
		/*7*/		-1.0f, -1.0f, 1.0f,	    
	};

	unsigned int lightElements[] = {
		0, 1, 3,
		1, 2, 3,

		0, 4, 1,
		1, 4, 5,

		0, 3, 7,
		0, 7, 4,

		1, 6, 2,
		1, 5, 6,

		2, 7, 3,
		2, 6, 7,

		4, 7, 5,
		5, 7, 6
	};

	Shader* shader = new Shader("vert.glsl", "frag.glsl");
	Shader* lightShader = new Shader("lightVert.glsl", "lightFrag.glsl");
	Transform* cubeTransform = new Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f));

	TextureLoader* textureLoader = new TextureLoader();
	textureLoader->loadTexture("images\\woodenbox.jpg");

	playerCamera = new Camera(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), window, WIDTH, HEIGHT);


	unsigned int VBO;
	glGenBuffers(1, &VBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rows * (3 + 4 + 2), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, elements, GL_STATIC_DRAW);


	unsigned int lightVBO, lightVAO, lightEBO;
	glGenBuffers(1, &lightVBO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightEBO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rows * 3, lightVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, lightElements, GL_STATIC_DRAW);


	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		processInput(window);

		//создание видовой матрицы
		glm::mat4 view = glm::mat4(1.f);
		//указываю начальную позицию камеры
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
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

		//2 куб освещенный
		glBindVertexArray(lightVAO);
		glm::mat4 lightModel = glm::mat4(1.f);
		lightModel = glm::translate(lightModel, glm::vec3(1.f, 0.f, 0.f));
		lightModel = glm::scale(lightModel, glm::vec3(0.1f, 0.1f, 0.1f));
		lightShader->setFloatMat4("lightpvm", projection * view * lightModel);
		float objectColor[] = { 1.0f, 0.4f, 0.31f };
		float lightColor[] = { 0.5f, 0.7f, 0.91f };
		lightShader->setFloatVec("objectColor", objectColor, 3);
		lightShader->setFloatVec("lightColor", lightColor, 3);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

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
