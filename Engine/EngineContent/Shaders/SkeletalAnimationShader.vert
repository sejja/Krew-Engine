#version 430 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBitangent;
layout(location = 5) in ivec4 boneIds;
layout(location = 6) in vec4 boneWeights;

out vec3 fPosition;
out vec2 fUV;
out vec3 fNormal;
out vec3 fTangent;
out vec3 fBitangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform int bAvg;

const int MAX_BONES = 128;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform bool boneUpdate;

mat4 compute_skinning_mtx() {
	mat4 skin_mtx =					
	boneWeights.x * finalBonesMatrices[boneIds[0]] + 
	boneWeights.y * finalBonesMatrices[boneIds[1]] + 
	boneWeights.z * finalBonesMatrices[boneIds[2]] + 
	boneWeights.w * finalBonesMatrices[boneIds[3]] ;
	
	return skin_mtx;
}


void main()
{
	bool avg = bAvg == 1;
	fNormal = vNormal;
	fTangent = vTangent;
	fBitangent = vBitangent;
	fPosition = (M * vec4(vPosition, 1.0f)).xyz;

	mat4 model_view_proj = P * V * M;
	fUV = vUV;
	mat4 skin_mtx = compute_skinning_mtx();
	if (boneUpdate)
		gl_Position = model_view_proj * skin_mtx * vec4(vPosition, 1.0f);
	else
		gl_Position = model_view_proj * vec4(vPosition, 1.0f);

}