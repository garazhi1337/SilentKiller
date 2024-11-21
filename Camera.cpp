#include "Camera.h"

void Camera::onMouse(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed: y ranges bottom to top
	lastX = xpos;
	lastY = ypos;
	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	if (firstMouse)
	{
		xoffset = 0.f;
		yoffset = 0.f;
		firstMouse = false;
	}

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, GLFWwindow* window, float width, float height)
{
	this->cameraPos = cameraPos;
	this->cameraFront = cameraFront;
	this->cameraUp = cameraUp;
	this->width = width;
	this->height = height;
	lastX = width / 2.f;
	lastY = height / 2.f;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Camera::~Camera()
{
	free(this);
}

void Camera::setPos(glm::vec3 cameraPos)
{
	this->cameraPos = cameraPos;
}

void Camera::setFront(glm::vec3 cameraFront)
{
	this->cameraFront = cameraFront;
}

void Camera::setUp(glm::vec3 cameraUp)
{
	this->cameraUp = cameraUp;
}

glm::vec3 Camera::getPos()
{
	return cameraPos;
}

glm::vec3 Camera::getFront()
{
	return cameraFront;
}

glm::vec3 Camera::getUp()
{
	return cameraUp;
}

glm::mat4 Camera::rotate()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::move(GLFWwindow* window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	//std::cout << "FPS: " << 1.f / deltaTime << std::endl;

	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
