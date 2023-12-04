#version 430 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 perspective_matrix;

out vec3 tex_coord;

void main() {
	tex_coord = vertex_position;
	gl_Position = perspective_matrix * camera_matrix * model_matrix * vec4(vertex_position, 1);
}