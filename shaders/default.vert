#version 430 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 transform;
out vec2 tex_coord;

void main()
{
	gl_Position = transform * vec4(vertexPosition, 1.0);
	tex_coord = vertexTexCoord;
}
