#version 330 core

layout (location = 0) in vec3 aPos1;
layout (location = 1) in vec3 normalCoords1;
layout (location = 2) in vec2 textCoords1;

uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

void main()
{
	gl_Position = p * v * m * vec4(aPos1, 1.0f);
}