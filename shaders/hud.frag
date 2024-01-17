#version 430 core

in vec2 fragTextureCoord;

out vec4 color;

uniform sampler2D hudTexture;
uniform float alphaMod;

void main()
{
    vec4 sampledColor = texture(hudTexture, fragTextureCoord);
    sampledColor.a = sampledColor.a * alphaMod;

    color = sampledColor;
}