#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	// strength
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 vs_normal;
in vec3 vs_fragPos;

out vec4 color;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	vec3 norm = normalize(vs_normal);
	vec3 lightDirection = normalize(light.position - vs_fragPos);

	float diffuseCoefficient = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * (diffuseCoefficient * material.diffuse);

	// specular
	vec3 viewDir = normalize(viewPos - vs_fragPos);
	vec3 reflectDir = reflect(-lightDirection, norm);

	float specularCoefficient = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (material.specular * specularCoefficient);  

	vec3 result = ambient + diffuse + specular;

	color = vec4(result, 1.0f);
}