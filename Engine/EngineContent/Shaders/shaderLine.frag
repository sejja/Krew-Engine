#version 330

in vec4 fColor;
out vec4 color;	// final fragment color

void main()
{
	color = fColor; // just the color
}