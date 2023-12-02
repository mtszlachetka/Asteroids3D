#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D earth_tex;

void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec4 tex_color = texture(earth_tex, tex_coord_flipped);
	out_color = tex_color;
}
