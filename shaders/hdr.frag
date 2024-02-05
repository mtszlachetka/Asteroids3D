#version 430 core

uniform sampler2D tex;
uniform sampler2D bloomed;

in vec2 tex_coord;
out vec4 out_color;

void main() {
	vec3 hdr_color = texture(tex, tex_coord).xyz;
	vec3 bloom_color = texture(bloomed, tex_coord).xyz;
	hdr_color += bloom_color;
	vec3 tone = 1 - exp(-hdr_color);
	out_color = vec4(tone, 1);
}