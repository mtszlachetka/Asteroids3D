#version 430 core

in vec3 view_dir_TS;

out vec4 out_color;

void main() {

	vec3 n = vec3(0, 0, 1);

	vec3 v = normalize(view_dir_TS);

	float hue = max(dot(-v, n), 0);

	out_color = vec4(mix(vec3(0.9, 0.9, 0.2), vec3(1, 0, 0), 1- hue), 1);
}