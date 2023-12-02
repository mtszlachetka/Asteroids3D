#version 430 core

in vec3 tex_coord;

uniform samplerCube skybox;

out vec4 out_color;

void main() {
	out_color = texture(skybox, tex_coord);
}