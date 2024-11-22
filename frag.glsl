#version 330

out vec4 outColor;
in vec2 textureCoords;

uniform vec3 color;
uniform sampler2D objTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec3 normal1;
in vec3 vertPos1;

void main()
{
	vec3 light = -normalize(vertPos1 - lightPos);
	vec3 norm = normalize(normal1);
	float diffuseCoef = max(dot(light, norm), 0.f);
	vec3 diffuseColor = lightColor * diffuseCoef;
	vec3 ambientColor = lightColor * 0.2f;

	outColor = texture(objTexture, textureCoords) * vec4(ambientColor + diffuseColor, 1.f);
	//outColor = vec4(color, 1.f);
}