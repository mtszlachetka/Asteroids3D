#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;

uniform mat4 transform;
uniform mat4 model;
uniform vec3 camera_pos;

out vec3 view_dir_TS;

void main() {

	mat3 TBN = transpose(mat3(vertex_tangent, vertex_bitangent, vertex_normal));

	vec3 view_dir = normalize(vertex_position - camera_pos);
	
	view_dir_TS = TBN * view_dir;

	gl_Position = transform * vec4(vertex_position, 1);
}