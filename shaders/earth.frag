#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D earth_tex;
uniform sampler2D clouds_tex;
uniform sampler2D earth_normals;

in vec3 light_dir_TS;
in vec3 view_dir_TS;

void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec4 tex_color = texture(earth_tex, tex_coord_flipped);
	vec4 clouds_color = texture(clouds_tex, tex_coord_flipped);
	vec3 normal = normalize(texture(earth_normals, tex_coord_flipped).xyz);

	normal = normalize(normal * 2 - 1);

	vec3 light_dir = normalize(light_dir_TS);
	vec3 view_dir = normalize(view_dir_TS);

	float diffuse = max(0, dot(normal, light_dir));
	out_color = mix(tex_color, vec4(1), clouds_color) * diffuse;
}
