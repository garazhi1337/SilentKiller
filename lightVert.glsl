#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 lightpvm;

void main()
{
	gl_Position = lightpvm * vec4(position, 1.0f);
}