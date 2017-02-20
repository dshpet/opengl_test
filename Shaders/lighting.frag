#version 330 core

in vec3 vs_normal;
in vec3 vs_fragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

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

	vec3 result = (ambient + diffuse) * objectColor;

	color = vec4(result, 1.0f);
}