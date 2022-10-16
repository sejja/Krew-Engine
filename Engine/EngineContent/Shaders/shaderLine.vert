#version 330

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 vColor;

out vec4 fColor;
uniform mat4 M; // model
uniform mat4 V; // view
uniform mat4 P; // projection

void main()
{
	// vertex shaders must at least write to gl_Position
	gl_Position = (P * V * M * vec4(vPos, 1));

	fColor = vColor;
}