#version 430 core
layout (location = 0) in vec4 vVtxData; 

out vec2 mUV;

uniform mat4 M;
uniform mat4 P;
uniform mat4 V;
uniform int isHUD;

void main()
{
	if(isHUD == 1)
		gl_Position = P * M * vec4(vVtxData.xy, 0.0f, 1.0f);
	else
		gl_Position = P * V * M * vec4(vVtxData.xy, 0.0f, 1.0f);
    mUV = vVtxData.zw;
}  