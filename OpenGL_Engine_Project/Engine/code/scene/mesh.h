#pragma once
#include "../common/common.h"
#include "../render/shader/rnd_shader.h"
#include "../render/texture/rnd_texture.h"
#include "../resource/res_resource_model.h"

namespace scene {

    class Mesh {
    public:
        // mesh Data
        std::vector<res::Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<rnd::Texture>>      textures;
        unsigned int VAO;

        // constructor
        Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<rnd::Texture>> textures);
        ~Mesh();

        // render the mesh
        void draw(const rnd::Shader& shader);
    private:
        // render data 
        unsigned int VBO, EBO;
        // initializes all the buffer objects/arrays
        void setupMesh();
    };
}