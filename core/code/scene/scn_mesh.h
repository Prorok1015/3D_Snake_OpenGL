#pragma once
#include <common.h>
#include <shader/rnd_shader.h>
#include <texture/rnd_texture.h>
#include <res_resource_model.h>
#include <rnd_material.h>

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