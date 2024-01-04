#version 430 core

in vec2 fragTextureCoord;

out vec4 color;

uniform sampler2D hudTexture;

void main()
{
    vec4 sampledColor = texture(hudTexture, fragTextureCoord);

    color = sampledColor;
}