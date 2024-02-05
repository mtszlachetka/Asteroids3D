#version 430 core

in vec3 tex_coord;

uniform samplerCube skybox;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 bright_color;

void main() {
	out_color = texture(skybox, tex_coord);
	bright_color = vec4(0);
}