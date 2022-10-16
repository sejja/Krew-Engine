#version 430
in  vec2 TexCoords;
out vec4 color;

uniform sampler2D sceneTex;
uniform sampler2D bloomTex;

uniform int bBloom;
uniform int bHDR;
uniform int bAces;
uniform float exposure;
uniform float gamma;
uniform int samples;

// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
vec3 aces(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
    color = texture(sceneTex, TexCoords);

    if(bBloom == 1)
    {
       for(int i = samples; i > 0; --i)
            color += vec4(textureLod(bloomTex, TexCoords, i).rgb, 1);
    }
    
    if(bHDR == 1)
    {
        if(bAces == 1)
        {
            color = vec4(aces(color.rgb), 1);
        }
        else
        {
            // exposure 
            vec3 mapped = vec3(1.0f) - exp(-color.rgb * exposure);
            // gamma correction 
            mapped = pow(mapped, vec3(1.0f / gamma));
            color = vec4(mapped, 1.0f);
        }
    }
}
