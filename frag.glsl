#version 330 core

out vec4 outColor;

uniform vec3 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

void main()
{
	outColor = vec4(color, 1.f);
}