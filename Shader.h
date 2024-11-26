#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

class Shader
{
	//тут будет описано создание шейдерной программы
public:
	Shader(std::string vertexPath, std::string fragmentPath);
	~Shader();
	void useProgram();
	unsigned int getProgram();
	void setFloatVec(std::string uniformPath, float* vec, int vecSize);
	void setFloatMat4(std::string uniformPath, glm::mat4 matrix);
	void setVec4(std::string uniformPath, glm::vec4 vector);
	void setVec3(std::string uniformPath, glm::vec3 vector);
	void setFloat(std::string uniformPath, float num);
	void setInt(std::string uniformPath, int num);
private:
	unsigned int program;
};

