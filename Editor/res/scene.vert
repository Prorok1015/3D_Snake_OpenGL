#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec4 aBonesWeight;
layout (location = 6) in ivec2 aTextureRowOffset;

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
} PS;

uniform mat4 model;

const int MAX_BONES = 200;
uniform int bone_row_height;
uniform int original_height;
uniform mat4 gBones[MAX_BONES];
layout(binding = 3) uniform isampler2D boneIndicesTexture;
const int MIPMAPLVL0 = 0;


// DEBUG REGION
uniform int use_animation;
uniform int draw_id;
out vec4 bones_wieght;
// DEBUG REGION END

int GetBonesHeight()
{
    return original_height;
//    int count = 0;
//    int boneIndex = -2;
//    do{
//        boneIndex = texelFetch(boneIndicesTexture, ivec2(0, count), MIPMAPLVL0).r;
//        count++;
//    } while(boneIndex > -2);
//
//    return count;
}

int GetBoneIndex(int x, int y)
{
    ivec2 boneCoord = ivec2(x, y);
    ivec2 texSize = textureSize(boneIndicesTexture, MIPMAPLVL0);
    //if (boneCoord.x > (texSize.x - 1))
    //{
        int power = boneCoord.x / texSize.x;
        int tmpX = boneCoord.x - (texSize.x * power);
        int tmpY = boneCoord.y * bone_row_height + power;
        boneCoord = ivec2(tmpX, tmpY);
    //}
    return texelFetch(boneIndicesTexture, boneCoord, MIPMAPLVL0).r;
}

mat4 culcSkinMatrix()
{
    bool is_at_least_one_bone = false;
    mat4 skinMatrix = mat4(0.0);

    int boneIndex = -1;
    const int count = GetBonesHeight();

    for(int i = 0; i < count; ++i)
    {
        boneIndex = GetBoneIndex(gl_VertexID, i);

        if(boneIndex > -1) {
            skinMatrix += gBones[boneIndex] * aBonesWeight[i];
            is_at_least_one_bone = true;
        } else {
            break;
        }
    }

    if (is_at_least_one_bone)
        return skinMatrix;

    return mat4(1.0);
}

void main()
{
    mat4 mesh = mat4(1.0);
    if (use_animation == 1) {
        mesh = culcSkinMatrix();
    }
    bones_wieght = aBonesWeight;
    PS.UV = aTexCoords;
    PS.FragPos = vec3(model * vec4(aPos, 1.0));
    PS.Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * mesh * vec4(aPos, 1.0);
}