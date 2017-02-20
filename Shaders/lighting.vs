#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vs_fragPos;
out vec3 vs_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 pos = vec4(position, 1.0f);
	
	vs_fragPos = vec3(model * pos);
	vs_normal = mat3(transpose(inverse(model))) * normal;

    gl_Position = projection * view * model * pos;
}