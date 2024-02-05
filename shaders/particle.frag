#version 430 core

uniform sampler2D particle1;
uniform sampler2D particle2;

in vec2 tex_coords;
in float tex_mix_ratio;
out vec4 fragOutput;

void main()
{
	fragOutput = mix(texture(particle1, tex_coords), texture(particle2, tex_coords), tex_mix_ratio);
    
	if (fragOutput.a <= 0.2) {
		discard;
	}

	fragOutput = vec4(vec3(10 * fragOutput.xyz), fragOutput.a);
}