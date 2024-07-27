#pragma once
#include "../common/common.h"
#include "../render/shader/rnd_shader.h"
#include "../render/texture/rnd_texture.h"
#include "../resource/res_resource_model.h"
#include "../render/rnd_material.h"

namespace scene {

    class Mesh {
    public:
        // mesh Data
        std::vector<res::Vertex>       vertices;
        std::vector<unsigned int> indices;

        rnd::Material material;

        // constructor
        Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices, const res::Tag& textures);
        Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices);
        ~Mesh();
    private:
    };
}