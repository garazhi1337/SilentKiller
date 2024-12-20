#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normalCoords;
layout (location = 2) in vec2 textCoords;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

out vec2 textureCoords;
out vec3 vertPosition;
out vec3 vertNormal;

void main()
{
	gl_Position = p * v * m * vec4(aPos, 1.0f);
	textureCoords = textCoords;
	vertPosition = vec3(m * vec4(aPos, 1.0f));
	vertNormal = vec3(m * vec4(normalCoords, 1.0f));
}