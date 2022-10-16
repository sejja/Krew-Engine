#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vUV;

out vec2 fUV;

uniform mat4 mtx;

void main()
{
    gl_Position = mtx * vec4(vPosition, 1.0f);
    fUV = vUV;
}