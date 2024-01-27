#version 430 core

out vec4 out_color;

uniform vec3 color;

void main()
{
	out_color = vec4(color.x, color.y, color.z, 1.0f);
}