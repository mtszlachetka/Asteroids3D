#version 430 core

in vec4 world_pos;

uniform vec3 light_vec;
uniform float far;

void main() {
	float distance = length(world_pos.xyz - light_vec);

	distance /= far;

	gl_FragDepth = distance;
}