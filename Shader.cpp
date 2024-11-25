#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	std::ifstream in;
	std::string vertexSource;
	std::string fragmentSource;
	const char* v;
	const char* f;

	in.open(vertexShaderPath);
	if (in.is_open())
	{
		while (!in.eof())
		{
			std::string tempVertexSource;
			std::getline(in, tempVertexSource);
			vertexSource += tempVertexSource + "\n";
		}
	}
	else
	{
		std::cout << "Не удалось открыть файл с Vertex шейдером *quq quq*" << std::endl;
	}

	in.close();
	
	in.open(fragmentShaderPath);
	if (in.is_open())
	{
		while (!in.eof())
		{
			std::string tempFragmentSource;
			std::getline(in, tempFragmentSource);
			fragmentSource += tempFragmentSource + "\n";
		}
	}
	else
	{
		std::cout << "Не удалось открыть файл с Fragment шейдером *quq quq*" << std::endl;
	}

	in.close();
	
	v = vertexSource.c_str();
	f = fragmentSource.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &f, NULL);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::useProgram()
{
	glUseProgram(program);
}

unsigned int Shader::getProgram()
{
	return program;
}

void Shader::setFloatVec(std::string uniformPath, float* vec, int vecSize)
{
	switch (vecSize)
	{
		case 1:
			glUniform1f(glGetUniformLocation(program, uniformPath.c_str()), vec[0]);
			break;
		case 2:
			glUniform2f(glGetUniformLocation(program, uniformPath.c_str()), vec[0], vec[1]);
			break;
		case 3:
			glUniform3f(glGetUniformLocation(program, uniformPath.c_str()), vec[0], vec[1], vec[2]);
			break;
		case 4:
			glUniform4f(glGetUniformLocation(program, uniformPath.c_str()), vec[0], vec[1], vec[2], vec[3]);
			break;
			
	}
}

void Shader::setFloatMat4(std::string uniformPath, glm::mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(program, uniformPath.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec4(std::string uniformPath, glm::vec4 vector)
{
	glUniform4f(glGetUniformLocation(program, uniformPath.c_str()), vector.x, vector.y, vector.z, vector.w);
}

void Shader::setVec3(std::string uniformPath, glm::vec3 vector)
{
	glUniform3f(glGetUniformLocation(program, uniformPath.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setFloat(std::string uniformPath, float num)
{
	glUniform1f(glGetUniformLocation(program, uniformPath.c_str()), num);
}
