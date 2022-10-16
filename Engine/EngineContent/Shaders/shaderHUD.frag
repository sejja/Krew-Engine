#version 430

in vec2 fUV;

layout (location = 0) out vec4 FragColor;

uniform sampler2D colorsTex;
uniform sampler2D normalMap;

void main()
{
	vec4 color = texture(colorsTex, fUV).rgba;
	if(color.a < 0.1f)
		discard;
	FragColor = color;
}