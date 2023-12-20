#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

uniform mat4 transforms[6];

out vec4 world_pos;

void main() {
	for (int face = 0; face < 6; face++) {
		gl_Layer = face;
		for (int i = 0; i < 3; i++) {
			world_pos = gl_in[i].gl_Position;
			gl_Position = transforms[face] * world_pos;
			EmitVertex();
		}
		EndPrimitive();
	}
}