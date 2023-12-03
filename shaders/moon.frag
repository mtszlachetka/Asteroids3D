#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D moon_tex;
uniform sampler2D moon_normals;

in vec3 light_dir_TS;
in vec3 view_dir_TS;
in float distance;

void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec4 tex_color = texture(moon_tex, tex_coord_flipped);
	vec3 normal = normalize(texture(moon_normals, tex_coord_flipped).xyz);

	normal = normal * 2 - 1;

	vec3 light_dir = normalize(light_dir_TS);
	vec3 view_dir = normalize(view_dir_TS);

	float diffuse = max(0, dot(normal, light_dir));
	vec3 reflection = reflect(light_dir, normal);
	float specular = pow(max(dot(view_dir, reflection), 0), 240);

	vec4 color = (tex_color * diffuse + specular) / pow(distance * 10, 2);
	float exposition = 3000;
	out_color = 1 - exp(-color * exposition);
}
