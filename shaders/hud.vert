#version 430 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 textureCoord;

out vec2 fragTextureCoord;

void main()
{
    gl_Position = vertexPosition;
    fragTextureCoord = textureCoord;
}