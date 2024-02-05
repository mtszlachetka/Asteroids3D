#version 430 core
uniform sampler2D tex;
in vec2 tex_coord;
out vec4 out_color;
void main() {
	out_color = texture(tex, tex_coord);
}