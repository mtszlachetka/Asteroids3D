#version 430 core

out vec4 out_color;
in vec2 tex_coord;

uniform sampler2D normal_map;
uniform sampler2D albedo_map;
uniform sampler2D amr_map;

uniform float exposition;

in vec3 light_dir_TS;
in vec3 view_dir_TS;
in float distance;

const float PI = 3.14159265;

float normal_distribution(vec3 normal, vec3 halfvec, float roughness) {
	return pow(roughness, 2) / (PI * pow(pow(dot(normal, halfvec), 2) * (pow(roughness, 2) - 1) + 1, 2));
}

float schlickGGX(vec3 normal, vec3 dir, float k) {
	return max(dot(normal, dir), 0) / (max(dot(normal, dir), 0) * (1 - k) + k);
}

float geometryGGX(vec3 normal, vec3 view_dir, vec3 light_dir, float k) {
	return schlickGGX(normal, view_dir, k) * schlickGGX(normal, light_dir, k);
}

vec3 fresnel_schlick(vec3 halfvec, vec3 view_dir, vec3 F0) {
	return F0 + (1 - F0) * pow(1 - max(dot(halfvec, view_dir), 0), 5);
}

vec3 pbr_light(vec3 light_dir, vec3 normal, vec3 view_dir, vec3 albedo, float metallic, float roughness, vec3 F0) {

	vec3 halfvec = normalize(view_dir + light_dir);

	float D = normal_distribution(normal, halfvec, roughness);
	float G = geometryGGX(normal, view_dir, light_dir, pow(roughness + 1, 2) / 8);
	vec3 F = fresnel_schlick(halfvec, view_dir, F0);

	vec3 ks = F;
	vec3 kd = (1 - F) * (1 - metallic);

	vec3 lambertian = kd * albedo / PI;
	vec3 cook_torrance = ks * D * G * F / (4 * dot(view_dir, normal) * dot(light_dir, normal) + 0.0000001);

	return (lambertian + cook_torrance) * max(dot(light_dir, normal), 0);
}

void main()
{
	vec2 tex_coord_flipped = vec2(tex_coord.x, -tex_coord.y);

	vec3 albedo = texture(albedo_map, tex_coord_flipped).xyz;
	vec3 normal = texture(normal_map, tex_coord_flipped).xyz;
	vec3 amr = texture(amr_map, tex_coord_flipped).xyz;
	normal = normal * 2 - 1;

	float AO = amr.r;
	float metallic = amr.g;
	float roughness = amr.b;

	float attentuation = 1 / pow(distance, 2);
	vec3 radiance = albedo * attentuation;

	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	vec3 light_dir = normalize(light_dir_TS);
	vec3 view_dir = normalize(view_dir_TS);

	
	vec3 color = pbr_light(light_dir, normal, view_dir, albedo, metallic, roughness, F0) * AO * attentuation;

	
	out_color = 1 - vec4(exp(-color * exposition), 1);
}
