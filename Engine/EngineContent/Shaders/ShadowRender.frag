#version 430

in vec2 fUV;

out vec3 FragColor;

uniform sampler2D texture0;
uniform bool bShadow;

void main()
{
	if (bShadow)
		FragColor = (texture(texture0, fUV).rrr - 0.99f) * 100.0f;
	else
		FragColor = texture(texture0, fUV).rgb;
}