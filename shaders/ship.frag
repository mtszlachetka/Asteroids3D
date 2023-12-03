#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D ship_tex;
uniform sampler2D scratches_tex;
uniform sampler2D rust_tex;
uniform sampler2D ship_normals;
uniform sampler2D rust_normals;

in vec3 light_dir_TS;
in vec3 view_dir_TS;
in float distance;


void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec4 tex_color = texture(ship_tex, tex_coord_flipped);
	vec4 scratches_color = texture(scratches_tex, tex_coord_flipped);
	vec4 rust_color = texture(rust_tex, tex_coord_flipped);
	vec3 ship_normal = texture(ship_normals, tex_coord_flipped).xyz;
	vec3 rust_normal = texture(rust_normals, tex_coord_flipped).xyz;

	vec3 light_dir = normalize(light_dir_TS);
	vec3 view_dir = normalize(view_dir_TS);
	vec3 normal = normalize(mix(ship_normal, rust_normal, rust_color.r));

	normal = normal * 2 - 1;

	float diffuse = max(0, dot(normal, light_dir));
	vec3 reflection = reflect(light_dir, normal);
	float specular = pow(max(dot(view_dir, reflection), 0), 240);
	
	vec4 color = (mix(tex_color, scratches_color, rust_color.r) * diffuse + specular) / pow(distance * 10, 2);
	float exposition = 3000;
	
	out_color = 1 - exp(-color * exposition);
}
