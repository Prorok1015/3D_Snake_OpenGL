#ifdef USE_ANIMATION
#define MAX_BONE_COUNT (boneCount < 5 ? boneCount : 4) // temporary while max bone weight == 4

layout (std140, binding = 2) uniform BoneMatrices // TODO: perhaps change to SSBO 
{
    int rowHeight;
    int boneCount;
    mat4 bones[MAX_BONE_MATRICES_COUNT];
};

layout(binding = 3) uniform isampler2D boneIndicesTexture;
const int MIPMAPLVL0 = 0;


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
#endif
