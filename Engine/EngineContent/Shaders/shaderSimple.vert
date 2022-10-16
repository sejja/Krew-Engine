#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vFaceNormal;
layout(location = 3) in vec3 vFaceTangent;
layout(location = 4) in vec3 vFaceBitangent;
layout(location = 5) in vec3 vAvgNormal;
layout(location = 6) in vec3 vAvgTangent;
layout(location = 7) in vec3 vAvgBitangent;

out vec3 fPosition;
out vec2 fUV;
out vec3 fNormal;
out vec3 fTangent;
out vec3 fBitangent;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform int bAvg;

void main()
{
    bool avg = bAvg == 1;
    fNormal = avg ? vAvgNormal : vFaceNormal;
    fTangent = avg ? vAvgTangent : vFaceTangent;
    fBitangent = avg ? vAvgBitangent : vFaceBitangent;
    fPosition = (M * vec4(vPosition, 1.0f)).xyz;
    fUV = vUV;
    gl_Position = P * V * vec4(fPosition, 1.0f);
}
