#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 vert_color;
out vec2 tex_coord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);
    vert_color = color;
	tex_coord = texCoord;
}