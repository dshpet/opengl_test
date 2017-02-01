#version 330 core

in vec3 vert_color;
in vec2 tex_coord;

out vec4 color;

uniform sampler2D assigned_texture1;
uniform sampler2D assigned_texture2;
uniform float blending;

void main()
{
	color = mix(texture(assigned_texture1, tex_coord), texture(assigned_texture2, vec2(tex_coord.x, 1.0f - tex_coord.y)), blending);
}