#version 430 core

uniform sampler2D particle1;
uniform sampler2D particle2;

in vec2 tex_coords;
in flat int tex_num;
out vec4 fragOutput;

void main()
{
	if (tex_num == 0) {
		fragOutput = texture(particle1, tex_coords);
	} else {
		fragOutput = texture(particle2, tex_coords);
	}
    
	if (fragOutput.a <= 0.2) {
		discard;
	}
}