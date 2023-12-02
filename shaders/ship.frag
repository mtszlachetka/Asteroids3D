#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D ship_tex;
uniform sampler2D scratches_tex;
uniform sampler2D rust_tex;

in vec3 light_dir_TS;
in vec3 view_dir_TS;


void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec4 tex_color = texture(ship_tex, tex_coord_flipped);
	vec4 scratches_color = texture(scratches_tex, tex_coord_flipped);
	vec4 rust_color = texture(rust_tex, tex_coord_flipped);

	vec3 light_dir = normalize(light_dir_TS);
	vec3 view_dir = normalize(view_dir_TS);
	vec3 normal = vec3(0, 0, 1);

	float diffuse = max(0, dot(normal, light_dir));
	out_color = mix(tex_color, scratches_color, rust_color.r) * diffuse;
}
