#version 330 core

in vec3 vs_normal;
in vec3 vs_fragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(vs_normal);
	vec3 lightDirection = normalize(lightPos - vs_fragPos);

	float diffuseCoefficient = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diffuseCoefficient * lightColor;

	// specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - vs_fragPos);
	vec3 reflectDir = reflect(-lightDirection, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	vec3 result = (ambient + diffuse + specular) * objectColor;

	color = vec4(result, 1.0f);
}