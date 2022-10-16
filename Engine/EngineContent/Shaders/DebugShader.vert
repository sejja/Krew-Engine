#version 440 core
layout(location = 0) in vec3 aPosition;
layout(location = 0) uniform mat4 M;
layout(location = 1) uniform mat4 V;
layout(location = 2) uniform mat4 P;

void main()
{
    vec4 Vertex = vec4(aPosition, 1.0f);
    gl_Position = P * V * M * Vertex;
}