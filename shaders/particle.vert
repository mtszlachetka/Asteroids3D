#version 430 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec4 tint;
layout (location = 2) in mat4 instance_matrix;

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;

out vec4 fragColor;


void main()
{
    gl_Position = projection_matrix * camera_matrix * instance_matrix * vec4(vertex_position, 1.0);
    fragColor = tint;
}