#version 330 core

in vec3 vert_color;
in vec2 tex_coord;

out vec4 color;

uniform sampler2D assigned_texture1;
uniform sampler2D assigned_texture2;

void main()
{
	color = mix(texture(assigned_texture1, tex_coord), texture(assigned_texture2, tex_coord), 0.2);
}