#version 330 core

out vec4 outColor;
in vec2 textureCoords;

uniform vec3 cameraPos;

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light
{
	vec3 direction; //для направленного света
	vec3 position; //для прожекторного и точечного света
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	int mode; //1 - направленный свет 2 - точечный свет 3 - прожектор
	float outerRadius;
	float innerRadius;
	vec3 spotlightDir;
	vec3 cameraDir;
};
uniform Light light;

in vec3 normal1;
in vec3 vertPos1;

void main()
{
	if (light.mode == 1)
	{
		vec3 ambientColor = light.ambient * vec3(texture(material.diffuseMap, textureCoords)) * material.ambient;

		vec3 lightVec = -normalize(light.direction);
		vec3 norm = normalize(normal1);
		float diffuseCoef = max(dot(lightVec, norm), 0.f);
		vec3 diffuseColor = diffuseCoef * light.diffuse * vec3(texture(material.diffuseMap, textureCoords)) * material.diffuse;

		vec3 reflectedLight = reflect(-lightVec, norm);
		vec3 view = -normalize(vertPos1 - cameraPos);
		float specularCoef = max(pow(dot(reflectedLight, view), material.shininess * 128.f), 0.f);
		vec3 specularColor = specularCoef * light.specular * vec3(texture(material.specularMap, textureCoords)) * material.specular;

		outColor = vec4(ambientColor + diffuseColor + specularColor, 1.f);
	}

	else if (light.mode == 2)
	{
		vec3 ambientColor = light.ambient * vec3(texture(material.diffuseMap, textureCoords)) * material.ambient;

		vec3 lightVec = -normalize(vertPos1 - light.position);
		vec3 norm = normalize(normal1);
		float diffuseCoef = max(dot(lightVec, norm), 0.f);
		vec3 diffuseColor = diffuseCoef * light.diffuse * vec3(texture(material.diffuseMap, textureCoords)) * material.diffuse;

		vec3 reflectedLight = reflect(-lightVec, norm);
		vec3 view = -normalize(vertPos1 - cameraPos);
		float specularCoef = max(pow(dot(reflectedLight, view), material.shininess * 128.f), 0.f);
		vec3 specularColor = specularCoef * light.specular * vec3(texture(material.specularMap, textureCoords)) * material.specular;

		float dist = length(light.position - vertPos1);
		float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

		ambientColor *= attenuation;
		diffuseColor *= attenuation;
		specularColor *= attenuation;

		outColor = vec4(ambientColor + diffuseColor + specularColor, 1.f);
	}
	if (light.mode == 3)
	{
		vec3 ambientColor = light.ambient * vec3(texture(material.diffuseMap, textureCoords)) * material.ambient;

		vec3 lightVec = -normalize(vertPos1 - light.position);
		float theta = acos(dot(normalize(light.cameraDir), -lightVec));
		vec3 norm = normalize(normal1);
		float diffuseCoef = max(dot(lightVec, norm), 0.f);
		vec3 diffuseColor = diffuseCoef * light.diffuse * vec3(texture(material.diffuseMap, textureCoords)) * material.diffuse;

		vec3 reflectedLight = reflect(-lightVec, norm);
		vec3 view = -normalize(vertPos1 - cameraPos);
		float specularCoef = max(pow(dot(reflectedLight, view), material.shininess * 128.f), 0.f);
		vec3 specularColor = specularCoef * light.specular * vec3(texture(material.specularMap, textureCoords)) * material.specular;

		float dist = length(light.position - vertPos1);
		float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

		ambientColor *= attenuation;
		diffuseColor *= attenuation;
		specularColor *= attenuation;

		if (theta < light.innerRadius)
		{
			outColor = vec4(ambientColor + diffuseColor + specularColor, 1.f);
		}
		else if (theta > light.innerRadius && theta < light.outerRadius)
		{
			float intensity = clamp((theta - light.outerRadius) / (light.innerRadius - light.outerRadius), 0.f, 1.f);
			diffuseColor *= intensity;
			specularColor *= intensity;
			outColor = vec4(ambientColor + diffuseColor + specularColor, 1.f);
		}
		else
		{
			outColor = vec4(ambientColor, 1.f);
		}
	}


}