#version 430 core

out vec4 out_color;

uniform vec3 color;
uniform float alpha;

void main()
{
	out_color = vec4(color.x, color.y, color.z, alpha);
}