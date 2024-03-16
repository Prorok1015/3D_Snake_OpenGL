#pragma once
#include "../common/common.h"
#include "../render/shader/shader.h"
#include "../render/texture/texture.h"
#include "../resource/res_resource_model.h"

namespace scene {

//constexpr auto MAX_BONE_INFLUENCE = 4;

    //struct Vertex {
    //    // position
    //    glm::vec3 Position;
    //    // normal
    //    glm::vec3 Normal;
    //    // texCoords
    //    glm::vec2 TexCoords;
    //    // tangent
    //    glm::vec3 Tangent;
    //    // bitangent
    //    glm::vec3 Bitangent;
    //    //bone indexes which will influence this vertex
    //    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //    //weights from each bone
    //    float m_Weights[MAX_BONE_INFLUENCE];
    //};

    class Mesh {
    public:
        // mesh Data
        std::vector<res::Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int VAO;

        // constructor
        Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        // render the mesh
        void Draw(Shader& shader);
        static void draw(const res::Mesh& mesh);
    private:
        // render data 
        unsigned int VBO, EBO;
        // initializes all the buffer objects/arrays
        void setupMesh();
    };
}