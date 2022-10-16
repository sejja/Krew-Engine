#version 430
layout (location = 0) in vec4 aModelPos;

out vec3 UV;

uniform mat4 V;
uniform mat4 P;
uniform mat4 uvRot;

void main()
{
    UV = mat3(uvRot) * vec3(aModelPos);
    gl_Position = P * V * aModelPos;
}  