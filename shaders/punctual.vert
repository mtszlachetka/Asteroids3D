#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_tex_coord;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;

uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_vec;
uniform vec3 camera_pos;

out VS_OUT {
	vec2 tex_coord;
	vec3 view_dir_TS;
	vec3 light_dir_TS;
	vec3 world_pos;
	float distance;
} vs_out;

void main()
{
	vs_out.world_pos = (model_matrix * vec4(vertex_position, 1)).xyz;
	vec3 normal = normalize(model_matrix * vec4(vertex_normal, 0)).xyz;
	vec3 tangent = normalize(model_matrix * vec4(vertex_tangent, 0)).xyz;
	vec3 bitagent = normalize(model_matrix * vec4(vertex_bitangent, 0)).xyz;

	mat3 TBN = transpose(mat3(tangent, bitagent, normal));

	vec3 view_dir = normalize(camera_pos - vs_out.world_pos);
	vec3 light_dir = normalize(light_vec - vs_out.world_pos);

	vs_out.view_dir_TS = TBN * view_dir;
	vs_out.light_dir_TS = TBN * light_dir;

	vs_out.distance = length(light_vec - vs_out.world_pos);

	gl_Position = projection_matrix * camera_matrix * model_matrix * vec4(vertex_position, 1.0);
	vs_out.tex_coord = vertex_tex_coord * vec2(1,-1);
}
