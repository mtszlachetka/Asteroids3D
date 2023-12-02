#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D earth_tex;
uniform sampler2D clouds_tex;

void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec4 tex_color = texture(earth_tex, tex_coord_flipped);
	vec4 clouds_color = texture(clouds_tex, tex_coord_flipped);

	out_color = mix(tex_color, vec4(1), clouds_color.r);
}
