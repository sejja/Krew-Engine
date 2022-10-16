#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vUV;

out vec2 fUV;
uniform mat4 M;
uniform mat4 P;

void main()
{
	fUV = vUV;
    gl_Position = P * M * vec4(vPosition.xy, 0.0f, 1.0f);
}
