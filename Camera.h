#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	void onMouse(GLFWwindow* window, double posx, double posy);
	Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, GLFWwindow* window, float width, float heigth);
	~Camera();
	void setPos(glm::vec3 cameraPos);
	void setFront(glm::vec3 cameraFront);
	void setUp(glm::vec3 cameraUp);
	glm::vec3 getPos();
	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::mat4 rotate();
	void move(GLFWwindow* window);

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float yaw = 90.0f;
	float pitch = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float width, height;
	float lastX;
	float lastY;
	bool firstMouse = true;
};

