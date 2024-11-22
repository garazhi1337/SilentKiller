#version 330

out vec4 outColor;
in vec2 textureCoords;

uniform vec3 color;
uniform sampler2D objTexture;
uniform vec3 lightPos;

in vec3 normalCoords;

void main()
{
	
	outColor = texture(objTexture, textureCoords) * vec4(color, 1.f);
	//outColor = vec4(color, 1.f);
}