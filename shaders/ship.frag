#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D ship_tex;
uniform sampler2D scratches_tex;
uniform sampler2D rust_tex;

void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec4 tex_color = texture(ship_tex, tex_coord_flipped);
	vec4 scratches_color = texture(scratches_tex, tex_coord_flipped);
	vec4 rust_color = texture(rust_tex, tex_coord_flipped);

	out_color = mix(tex_color, scratches_color, rust_color.r);
}
