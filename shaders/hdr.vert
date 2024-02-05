#version 430 core
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_tex_coord;
out vec2 tex_coord;
void main() {
	gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0, 1.0);
	tex_coord = vertex_tex_coord * vec2(1, -1);
}