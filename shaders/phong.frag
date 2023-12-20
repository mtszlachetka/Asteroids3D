#version 430 core

out vec4 out_color;

uniform vec3 light_vec;
uniform sampler2D shadow_map;

in VS_OUT {
	vec3 normal;
	vec3 world_pos;
	vec3 light_space_pos;
} fs_in;

void main() {
	vec3 n = normalize(fs_in.normal);

	float diffuse = max(dot(n, light_vec), 0);

	vec3 resized = fs_in.light_space_pos * 0.5 + 0.5;
	float closest = texture(shadow_map, resized.xy).r;
	float current = resized.z;

	float bias = 0.005;
	float shadow;
	if (current > 1.0) {
		shadow = 0.0;
	} else {
		shadow = current - bias > closest ? 1.0 : 0.0;
	}
	out_color = vec4(1) * diffuse * (1 - shadow);
}