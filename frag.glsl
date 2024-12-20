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
uniform vec3 eyePos;
uniform int lightMode;
uniform int useSpecular;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	//outColor = vec4(0.1, 0.2, 0.3, 1.f);
	if (lightMode == DIRECTIONAL)
	{
		vec4 ambient = texture(texture_diffuse1, textureCoords) * 0.15f;
		float diffuseCoef = max(dot(-normalize(dirLightPos), normalize(vertNormal)), 0.0f);
		vec4 diffuse = texture(texture_diffuse1, textureCoords) * diffuseCoef;
		float specularCoef = max(pow(dot(reflect(normalize(dirLightPos), vertNormal), -normalize((vertPosition - eyePos))), 64), 0.0f);
		vec4 specular = texture(texture_specular1, textureCoords) * specularCoef;
		if (useSpecular == 1)
		{
			outColor = ambient + diffuse + specular;
		}
		else
		{
			outColor = ambient + diffuse;
		}
		
	}
	else if (lightMode == PROJECTOR)
	{
		
	}
	else
	{

	}

}