#version 430 core

uniform sampler2D noise;

in vec4 fragColor;
in vec2 tex_coords;
out vec4 fragOutput;

void main()
{
    fragOutput = texture(noise, tex_coords);
}