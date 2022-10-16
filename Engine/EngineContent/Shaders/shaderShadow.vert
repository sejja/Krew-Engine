#version 430

layout(location = 0) in vec3 vPosition;

uniform mat4 lightMtx;
uniform mat4 M;

void main()
{
    gl_Position = lightMtx * M * vec4(vPosition, 1.0f);
}