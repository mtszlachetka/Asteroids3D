#version 430 core

uniform sampler2D tex;

in vec2 tex_coord;
out vec4 out_color;

void main() {
	vec3 hdr_color = texture(tex, tex_coord).xyz;
	vec3 mapped = hdr_color / (hdr_color + vec3(1));
	out_color = vec4(mapped, 1);
}