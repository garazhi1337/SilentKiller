#version 330

out vec4 outColor;
in vec2 textureCoords;

uniform vec3 uniformColor;
uniform sampler2D objTexture;

void main()
{
	outColor = texture(objTexture, textureCoords);
}