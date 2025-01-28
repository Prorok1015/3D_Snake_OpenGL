#version 460 core

#include "pipline_struct_lib.glsl"
#include "phong_model_lib.frag"

in PiplineStruct PS;

#ifdef USE_NORMAL_MAP
layout(binding = 2) uniform sampler2D normalTxm;
#endif

vec3 getNormalVector()
{
#ifdef USE_NORMAL_MAP
    vec3 normal = texture(normalTxm, PS.UV).rgb;
    normal = normalize(normal * 2.0 - 1.0); // from [0, 1] to [-1, 1]
    return normalize(PS.TBN * normal);
#else
    return normalize(PS.Normal);
#endif
}

vec3 getMaterialColor()
{
#ifdef USE_TXM_AS_DIFFUSE
    return texture(diffuseTxm, PS.UV).rgb;
#else
    return diffuseColor.rgb;
#endif
}

vec3 getMaterialSpecular()
{
#ifdef USE_SPECULAR_MAP
    return texture(specularTxm, PS.UV).rgb;
#else
    return vec3(0);
#endif
}

out vec4 fragColor;
void main()
{    
    vec3 norm = getNormalVector();
    vec3 result = calculatePhongModel(norm, getMaterialColor(), getMaterialSpecular(), PS.FragPos);
    fragColor = vec4(result, 1.0);
}