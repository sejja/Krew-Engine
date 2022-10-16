#version 440 core
out vec4 OutColor;
layout(location = 3) uniform vec4 Color;

void main()
{
    OutColor = Color;
}