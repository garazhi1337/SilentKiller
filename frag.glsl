#version 330 core

out vec4 outColor;
in vec2 textureCoords;

uniform vec3 color;
uniform sampler2D objTexture1;
uniform sampler2D objTexture2;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

in vec3 normal1;
in vec3 vertPos1;

void main()
{
	vec3 ambientColor = light.ambient * material.ambient;

	vec3 lightVec = -normalize(vertPos1 - light.position);
	vec3 norm = normalize(normal1);
	float diffuseCoef = max(dot(lightVec, norm), 0.f);
	vec3 diffuseColor = (material.diffuse * diffuseCoef) * light.diffuse;

	vec3 reflectedLight = reflect(-lightVec, norm);
	vec3 view = -normalize(vertPos1 - cameraPos);
	float specularCoef = max(pow(dot(reflectedLight, view), material.shininess * 128.f), 0.f);
	vec3 specularColor = (material.specular * specularCoef) * light.specular;
	if (color == vec3(0.f, 0.f, 0.f))
	{
		outColor = mix(texture(objTexture1, textureCoords), texture(objTexture2, textureCoords), 0.f) * vec4(ambientColor + diffuseColor + specularColor, 1.f);
		//outColor = vec4(ambientColor + diffuseColor + specularColor, 1.f);
	}
	else
	{
		outColor = vec4(color, 1.f);
	}
	//outColor = vec4(color, 1.f);
}