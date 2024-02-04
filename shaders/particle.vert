#version 430 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in int texture_num;
layout (location = 2) in mat4 instance_matrix;
layout (location = 6) in vec2 texture_coords;

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;

out vec2 tex_coords;
out flat int tex_num;


void main()
{
    gl_Position = projection_matrix * camera_matrix * instance_matrix * vec4(vertex_position, 1.0);
	tex_coords = texture_coords * vec2(1, -1);
	tex_num = texture_num;
}