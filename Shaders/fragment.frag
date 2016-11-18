#version 330 core

in vec4 pos;
in vec3 vert_color;

out vec4 color;

uniform vec4 defaultColor;

void main()
{
	color = defaultColor;
}