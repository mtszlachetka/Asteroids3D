#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_space_matrix;

out VS_OUT {
	vec3 normal;
	vec3 world_pos;
	vec3 light_space_pos;
} vs_out;

void main() {
	gl_Position = projection_matrix * camera_matrix * model_matrix * vec4(vertex_position, 1.0);
	vs_out.normal = vec3(model_matrix * vec4(vertex_normal, 0));
	vs_out.world_pos = vec3(model_matrix * vec4(vertex_position, 1));
	vs_out.light_space_pos = vec3(light_space_matrix * model_matrix * vec4(vertex_position, 1));
}