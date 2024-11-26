#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normalCoords;
layout (location = 2) in vec2 textCoords;

out vec2 textureCoords;
out vec3 normal1;
out vec3 vertPos1;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
	gl_Position = p * v * m * vec4(aPos, 1.0f);
	textureCoords = textCoords;
	normal1 = mat3(m) * normalCoords;
	vertPos1 = mat3(m) * aPos;
}