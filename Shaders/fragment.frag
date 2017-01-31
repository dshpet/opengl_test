#version 330 core

in vec3 vert_color;
in vec2 tex_coord;

out vec4 color;

uniform sampler2D assigned_texture;

void main()
{
	color = texture(assigned_texture, tex_coord);
}