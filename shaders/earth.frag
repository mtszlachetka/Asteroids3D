#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D earth_tex;
uniform sampler2D clouds_tex;
uniform sampler2D earth_normals;

in vec3 light_dir_TS;
in vec3 view_dir_TS;
in float distance;

float kd = 0.8;
float ks = 0.2;
float alpha = 0.8;
vec3 F0 = vec3(0.04, 0.4, 0.4);

float PI = 3.14159265;

float normal_distribution(vec3 normal, vec3 halfvec) {
	return pow(alpha, 2) / (PI * pow(pow(dot(normal, halfvec), 2) * (pow(alpha, 2) - 1) + 1, 2));
}

float schlickGGX(vec3 normal, vec3 dir, float k) {
	return max(dot(normal, dir), 0) / (max(dot(normal, dir), 0) * (1 - k) + k);
}

float geometryGGX(vec3 normal, vec3 view_dir, vec3 light_dir, float k) {
	return schlickGGX(normal, view_dir, k) * schlickGGX(normal, light_dir, k);
}

vec3 fresnel_schlick(vec3 halfvec, vec3 view_dir) {
	return F0 + (1 - F0) * pow(1 - max(dot(halfvec, view_dir), 0), 5);
}

vec3 pbr_light(vec3 light_dir, vec3 normal, vec3 view_dir, vec3 object_color) {
	vec3 lambertian = kd * object_color / PI;

	vec3 halfvec = normalize(view_dir + light_dir);

	float D = normal_distribution(normal, halfvec);
	float G = geometryGGX(normal, view_dir, light_dir, pow(alpha + 1, 2) / 8);
	vec3 F = fresnel_schlick(halfvec, view_dir);

	vec3 cook_torrance = ks * D * G * F / (4 * dot(view_dir, normal) * dot(light_dir, normal));

	return (lambertian + cook_torrance) * max(dot(light_dir, normal), 0);
}

void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec3 tex_color = texture(earth_tex, tex_coord_flipped).xyz;
	vec3 clouds_color = texture(clouds_tex, tex_coord_flipped).xyz;
	vec3 normal = normalize(texture(earth_normals, tex_coord_flipped).xyz);

	normal = normal * 2 - 1;

	vec3 light_dir = normalize(light_dir_TS);
	vec3 view_dir = normalize(view_dir_TS);
	
	vec3 color = pbr_light(light_dir, normal, view_dir, mix(tex_color, vec3(1), clouds_color.r)) / pow(distance * 10, 2);
	float exposition = 20000;

	out_color = 1 - vec4(exp(-color * exposition), 1);
}
