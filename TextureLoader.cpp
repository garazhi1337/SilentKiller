#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include "Libraries/include/stb_image.h"
#include "TextureLoader.h"

void TextureLoader::loadTexture(const char* texturePath)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &channels, 0);
	//std::cout << *data << std::endl;
	if (data)
	{
		//сделать в классе шейдер в униформу текстуры
		if (channels == 3)
		{
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		//пока что возможно наложение только 1 текстуры
		//glActiveTexture(GL_TEXTURE0);
	}
	else
	{
		std::cout << "Failed to load texture *quq quq*" << std::endl;
	}
	stbi_image_free(data);
}

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{
	free(this);
}
