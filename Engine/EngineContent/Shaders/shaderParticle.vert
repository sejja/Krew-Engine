#version 330 core
layout (location = 0) in vec3 mVtx; // <model vertices>
layout (location = 1) in vec4 mOffset; // <vec3 position, life>
// layout (location = 2) in vec4 color; // <vec2 position, vec2 texCoords>
// layout (location = 1) in vec2 texcoords; // <vec2 position, vec2 texCoords>
// layout (location = 2) in vec4 particlepos; // <vec2 position, vec2 texCoords>
// layout (location = 3) in vec4 specolor; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 P;
uniform mat4 V;
// uniform mat4 rot;
// uniform vec3 offset;
uniform vec4 color;
uniform float scale;

void main()
{
    mat4 translate = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(mOffset.xyz,1));
    // TexCoords = texcoords;
    ParticleColor = vec4(color.xyz, color.w * mOffset.w);
    gl_Position = P * V * translate * vec4((mVtx * scale), 1.0);
}