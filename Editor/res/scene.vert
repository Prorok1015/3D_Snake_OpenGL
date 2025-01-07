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

#define MAX_BONE_MATRICES_COUNT 128
#define MAX_BONE_COUNT (boneCount < 5 ? boneCount : 4) // temporary while max bone weight == 4

layout (std140, binding = 2) uniform BoneMatrices // TODO: perhaps change to SSBO 
{
    int rowHeight;
    int boneCount;
    mat4 bones[MAX_BONE_MATRICES_COUNT];
};

uniform mat4 uWorldModel;
uniform mat4 uWorldMeshMatr;
layout(binding = 3) uniform isampler2D boneIndicesTexture;
const int MIPMAPLVL0 = 0;


// DEBUG REGION
uniform int use_animation;
uniform int draw_id;
out vec4 bones_wieght;
// DEBUG REGION END

int getBoneIndex(int x, int y)
{
    ivec2 texSize = textureSize(boneIndicesTexture, MIPMAPLVL0);
    int power = x / texSize.x;
    int tmpX = x - (texSize.x * power);
    int tmpY = y * rowHeight + power;
    ivec2 boneCoord = ivec2(tmpX, tmpY);
    return texelFetch(boneIndicesTexture, boneCoord, MIPMAPLVL0).r;
}

mat4 culcSkinMatrix()
{
    bool is_at_least_one_bone = false;
    mat4 skinMatrix = mat4(0.0);

    int boneIndex = -1;

    for(int i = 0; i < MAX_BONE_COUNT; ++i)
    {
        boneIndex = getBoneIndex(gl_VertexID, i);

        if(boneIndex > -1) {
            skinMatrix += bones[boneIndex] * aBonesWeight[i];
            is_at_least_one_bone = true;
        } else {
            break;
        }
    }

    if (is_at_least_one_bone)
        return skinMatrix;

    return uWorldMeshMatr;
}

void main()
{
    mat4 mesh = uWorldMeshMatr;
    if (use_animation == 1) {
        mesh = culcSkinMatrix();
    }
    bones_wieght = aBonesWeight;
    PS.Color = aColor;
    PS.UV = aTexCoords;
    PS.FragPos = vec3(uWorldModel * vec4(aPos, 1.0));
    PS.Normal = mat3(transpose(inverse(uWorldModel))) * aNormal;
    gl_Position = projection * view * uWorldModel * mesh * vec4(aPos, 1.0);
}