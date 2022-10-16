#version 430 core

in vec2 mUV;
out vec4 OutColor;

uniform sampler2D Text;
uniform vec4 TextColor;

void main()
{    
    //Get a sample of the fragment & apply uniform color
    vec4 SampleC = vec4(1.0, 1.0, 1.0, texture(Text, mUV).r);
    vec4 finalcol = TextColor * SampleC;

    if(finalcol.a < 0.1)
        discard;

    OutColor = finalcol;  
}  