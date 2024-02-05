#version 430 core

uniform sampler2D particle1;
uniform sampler2D particle2;

in vec2 tex_coords;
in float tex_mix_ratio;
layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 bright_color;

void main()
{
	out_color = mix(texture(particle1, tex_coords), texture(particle2, tex_coords), tex_mix_ratio);
    
	if (out_color.a <= 0.2) {
		discard;
	}

	out_color = vec4(vec3(10 * out_color.xyz), out_color.a);
	bright_color = out_color;
}