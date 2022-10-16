#version 430

in vec3 UV;

out vec3 OutColor;
uniform samplerCube SkyboxTex;
uniform vec4 fogcolor;
uniform float fogintensity;

void main()
{
    OutColor = texture(SkyboxTex, vec3(-UV.x, UV.y, UV.z)).rgb * (fogcolor * fogintensity).rgb;
}