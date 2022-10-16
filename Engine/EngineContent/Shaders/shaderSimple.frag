#version 430

in vec3 fPosition;
in vec2 fUV;
in vec3 fNormal;
in vec3 fTangent;
in vec3 fBitangent;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

struct Light
{
    //int type;
    vec3 pos;
    vec3 dir;
    vec3 amb;
    vec3 dif;
    vec3 spe;
    vec3 att;
    float cosIn;
    float cosOut;
    float fallOff;
    float bias;
    int neighbor;
    mat4 mtx;
    sampler2D shadowMap;
};
uniform Light lights[8];

struct Material
{
    vec3 amb;
    vec3 dif;
    vec3 spe;
    float shiny;
    float dashImageVar;
};
uniform Material mat;

uniform int numLights;

uniform sampler2D colorsTex;
uniform sampler2D normalMap;

uniform float threshold;
uniform float bloomIntensity;
uniform int isLight;
uniform bool hasNormalMap;
uniform mat4 V;
uniform mat4 M;
uniform vec4 fogColor;

////////////////////////FOG
//Uniforms go brrrr

uniform float near;
uniform float far;
uniform float fogsteepness;
uniform float fogoffset;
uniform float fogintensity;
uniform int fogenabled;
uniform float fogdensity;
uniform int fogpreset;

/////////////////////////////

float ComputeShadow(int i, vec3 pos)
{
    vec4 lsPos = vec4(lights[i].mtx * vec4(pos, 1));
    vec3 projPos = (lsPos.xyz / lsPos.w) * 0.5f + 0.5f;
    float depth = projPos.z - lights[i].bias;
    
    vec2 offset = 1.0f / textureSize(lights[i].shadowMap, 0);
    int neighbor = lights[i].neighbor;
    float visibility = 0;
    float samples = 0;

    for (int x = -neighbor; x <= neighbor; x++)
    {
        for (int y = -neighbor; y <= neighbor; y++)
        {
            float shadowDepth = texture(lights[i].shadowMap, projPos.xy + offset * vec2(x, y)).r;
            visibility += shadowDepth > depth ? 1.0f : 0.0f;
            samples += 1.0f;
        }
    }

    return visibility / samples;
}

vec3 phongLighting()
{
    mat4 VM = V * M;
    mat3 VM3 = mat3(VM);
    mat3 iVM3 = inverse(transpose(VM3));
    vec3 N;
    if(hasNormalMap)
    {
        mat3 V_M_TBN = iVM3 * mat3(fTangent, fBitangent, fNormal);
        N = normalize(V_M_TBN * (texture(normalMap, fUV).rgb * 2.0f - 1.0f));
    }
    else
        N = normalize(iVM3 * fNormal);

    vec3 color;
    vec3 diffuse = texture(colorsTex, fUV).rgb;
    //vec3 diffuse = vec3(fUV, 0.0f);
    vec3 vmPos = (V * vec4(fPosition, 1)).xyz;
    vec3 V = normalize(-vmPos);
    for (int i = 0; i < numLights; i++)
    {
        float spotlight = 1.0f;
        vec3 PtoL = lights[i].pos - vmPos;
        float dist = length(PtoL);
        float att = 1.0f;
        vec3 L;
        //if (lights[i].type != 2) //Point or spot
        //{
            L = normalize(PtoL);
            att = min(1.0f / (lights[i].att.x + lights[i].att.y * dist + lights[i].att.z * dist * dist), 1.0f);
            //if (lights[i].type == 1) // Spot
                spotlight = pow((dot(normalize(lights[i].dir), -L) - lights[i].cosOut) / (lights[i].cosIn - lights[i].cosOut), lights[i].fallOff);
        //}
        //else //Directional
        //    L = normalize(-lights[i].dir);
        vec3 R = normalize(2.0f * dot(N, L) * N - L);

        float shadow = ComputeShadow(i, fPosition);

        vec3 lit = lights[i].amb * mat.amb * diffuse;

        float dotP = dot(V, R); // Phong
        //float dotP = dot(normalize(L+V), N); // Blinn-Phong

        lit += max(dot(N, L), 0.0f) * lights[i].dif * mat.dif * diffuse
             + max(pow(dotP, mat.shiny), 0.0f) * lights[i].spe * mat.spe;
        color += shadow * lit * att * clamp(spotlight, 0.0f, 1.0f);
    }
    return color;
}

float linearDepth(float depth) {
    return (2 * near * far) / (far  + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness, float offset) {
    float zVal = linearDepth(depth);
    return (1 / (1 + exp(-steepness * (zVal - offset))));
}

float exponentialdepth(float depth, float density) {
    return 1.0 - exp(-depth*density);
}

float exponentialsquareddepth(float depth, float density) {
    return 1.0 - exp(pow(-depth*density, 2));
}

void main()
{
    float depth = 0;

    switch(fogpreset) {
    case 0:
        depth = logisticDepth(gl_FragCoord.z, fogsteepness, fogoffset);
    break;
    case 1:
        depth = exponentialdepth(gl_FragCoord.z, fogdensity);
    break;
    case 2:
        depth = exponentialsquareddepth(gl_FragCoord.z, fogdensity);
    break;
    }

    if (isLight == 1)
        FragColor = vec4(1.0f);
    else if (numLights == 0) {
        vec4 albedo = texture(colorsTex, fUV).rgba;
        if(fogenabled >= 1)
            albedo = (albedo * (1.0f - depth) + vec4(depth * fogColor * fogintensity));
        FragColor = albedo;
        FragColor.a *= mat.dashImageVar;
    }
    else {
        vec4 albedo = vec4(phongLighting(), 1.0f);
        if(fogenabled >= 1)
            albedo = (albedo * (1.0f - depth) + vec4(depth * fogColor * fogintensity));
        FragColor = albedo;
    }
    FragColor += FragColor * bloomIntensity;
    // check whether fragment output is higher than threshold, if so output as brightness color
    //float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    //if(brightness > threshold)
        BloomColor = vec4(FragColor.rgb - threshold, FragColor.a);
        for(int i = 0; i < 3; ++i)
            if(BloomColor[i] < 0)
                BloomColor[i] = 0;
    //else
    //    BloomColor = vec4(0.0, 0.0, 0.0, 1.0);

}