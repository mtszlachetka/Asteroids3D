#version 430 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 transform;

out vec3 tex_coord;

void main() {
	tex_coord = vertex_position;
	gl_Position = transform * vec4(vertex_position, 1);
}