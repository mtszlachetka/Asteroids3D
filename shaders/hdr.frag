#version 430 core

uniform sampler2D tex;

in vec2 tex_coord;
out vec4 out_color;

void main() {
	vec3 hdr_color = texture(tex, tex_coord).xyz;
	vec3 tone = 1 - exp(-hdr_color * 1.2);
	out_color = vec4(tone, 1);
}