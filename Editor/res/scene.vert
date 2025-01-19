#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec4 aBonesWeight; //TODO: move to texture2D
layout (location = 6) in vec4 aColor;

layout (std140, binding = 0) uniform Matrices 
{
    mat4 projection;
    mat4 view;
    float time;
    vec3 view_position;
};

out struct PiplineStruct
{
    vec2 UV;
    vec3 Normal;
    vec3 FragPos;
    vec4 Color;
} PS;

#include "first_include.vert"
#include "second_include.vert"

void main()
{
    mat4 mesh = uWorldMeshMatr;
    #ifdef USE_ANIMATION 
        mesh = culcSkinMatrix();
    #endif
    PS.Color = aColor;
    PS.UV = aTexCoords;
    PS.FragPos = vec3(uWorldModel * vec4(aPos, 1.0));
    PS.Normal = mat3(transpose(inverse(uWorldModel))) * aNormal;
    gl_Position = projection * view * uWorldModel * mesh * vec4(aPos, 1.0);
}