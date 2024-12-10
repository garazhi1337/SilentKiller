#version 330 core

out vec4 outColor;
in vec2 textureCoords;

uniform vec3 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;
uniform sampler2D texture_diffuse6;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

void main()
{
	//outColor = vec4(0.1, 0.2, 0.3, 1.f);
	outColor = texture(texture_diffuse1, textureCoords);
}