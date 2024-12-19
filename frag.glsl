#version 330 core

#define DIRECTIONAL 1
#define PROJECTOR 2
#define POINT 3

out vec4 outColor;
in vec2 textureCoords;
in vec3 vertPosition;
in vec3 vertNormal;

uniform vec3 projectorLightPos;
uniform vec3 dirLightPos;
uniform vec3 pointLightPos;
uniform int lightMode;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	//outColor = vec4(0.1, 0.2, 0.3, 1.f);
	if (lightMode == DIRECTIONAL)
	{
		vec4 ambient = texture(texture_diffuse1, textureCoords) * 0.2f;
		float diffuseCoef = max(dot(-normalize(dirLightPos), vertNormal), 0.0f);
		vec4 diffuse = texture(texture_diffuse1, textureCoords) * diffuseCoef;
		float specularCoef = 0.0f;
		vec4 specular = texture(texture_specular1, textureCoords);
		outColor = specular;
	}
	else if (lightMode == PROJECTOR)
	{
		
	}
	else
	{

	}

}