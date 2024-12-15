#define DIRECTIONAL 1
#define PROJECTOR 2
#define POINT 3

#include "Game.h"
#include "Light.cpp"

Game::Game()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "SilentKiller", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

	playerCamera = new Camera(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), window, WIDTH, HEIGHT, 10.0f);
}

Game::~Game()
{
	delete this;
}

int Game::run()
{
	std::srand(std::time(nullptr));

	if (window == nullptr)
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

	Shader* shader = new Shader("vert.glsl", "frag.glsl");
	shader->useProgram();

	Model* ball = new Model("models\\shooting_range.obj");
	Light* light = new Light(shader);
	light->setLightMode(DIRECTIONAL);
	light->setDirLightPos(glm::vec3(- 1.0f, -1.0f, -1.0f));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.075f, 0.15f, 1.f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window);

		shader->useProgram();
		shader->setVec3("eyePos", playerCamera->getPos());

		//glBindVertexArray(VAO);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = playerCamera->rotate();
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);

		shader->setFloatMat4("m", model);
		shader->setFloatMat4("v", view);
		shader->setFloatMat4("p", projection);
		ball->Draw(shader);

		glfwSetWindowSizeCallback(window, handleOnResize);
		glfwSetCursorPosCallback(window, handleOnMouse);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void Game::onResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Game::onMouse(GLFWwindow* window, double posx, double posy)
{
	playerCamera->onMouse(window, posx, posy);
}

void Game::processInput(GLFWwindow* window)
{
	playerCamera->move(window);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float Game::randFloat(float min, float max)
{
	float num = min + rand() * (max - min) / RAND_MAX;
	return num;
}

glm::vec3 Game::randSphericPosition(float x0, float y0, float z0, float minRadius, float maxRadius)
{
	float x = randFloat(-maxRadius, maxRadius);
	float y = randFloat(-maxRadius, maxRadius);
	float z = randFloat(-maxRadius, maxRadius);
	float checkRadius = (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	while ((checkRadius < minRadius || checkRadius > maxRadius))
	{
		x = randFloat(-maxRadius, maxRadius);
		y = randFloat(-maxRadius, maxRadius);
		z = randFloat(-maxRadius, maxRadius);
		checkRadius = (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	return glm::vec3(x + x0, y + y0, z + z0);
}

static void handleOnResize(GLFWwindow* _window, int width, int height)
{
	cout << "kittens" << endl;
	reinterpret_cast<Game*>(glfwGetWindowUserPointer(_window))->onResize(_window, width, height);
}

static void handleOnMouse(GLFWwindow* _window, double xpos, double ypos)
{
	reinterpret_cast<Game*>(glfwGetWindowUserPointer(_window))->onMouse(_window, xpos, ypos);
}