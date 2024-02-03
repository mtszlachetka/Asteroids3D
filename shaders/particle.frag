#version 430 core

in vec3 fragColor;
out vec4 fragOutput;

void main()
{
    fragOutput = vec4(fragColor, 1.0);
}