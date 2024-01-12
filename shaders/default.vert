#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_tex_coord;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;


uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_space_matrix;

uniform vec3 light_dir;
uniform vec3 camera_pos;

out vec2 tex_coord;
out vec3 view_dir_TS;
out vec3 light_dir_TS;
out vec3 light_space_pos;

void main() {
	vec3 world_pos = (model_matrix * vec4(vertex_position, 1)).xyz;
	vec3 normal = normalize(model_matrix * vec4(vertex_normal, 0)).xyz;
	vec3 tangent = normalize(model_matrix * vec4(vertex_tangent, 0)).xyz;
	vec3 bitangent = normalize(model_matrix * vec4(vertex_bitangent, 0)).xyz;

	mat3 TBN = transpose(mat3(tangent, bitangent, normal));

	view_dir_TS = TBN * normalize(camera_pos - world_pos);
	light_dir_TS = TBN * normalize(light_dir);
	light_space_pos = vec3(light_space_matrix * model_matrix * vec4(vertex_position, 1));
	tex_coord = vertex_tex_coord * vec2(1, -1);
	gl_Position = projection_matrix * camera_matrix * model_matrix * vec4(vertex_position, 1.0);
}