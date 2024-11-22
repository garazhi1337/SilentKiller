#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 textCoords;

out vec2 textureCoords;

uniform mat4 pvm;

void main()
{
	gl_Position = pvm * vec4(aPos, 1.0f);
	textureCoords = textCoords;
}