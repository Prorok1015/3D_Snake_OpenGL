#pragma once
#include <common.h>
#include <texture/rnd_texture.h>
#include <res_resource_model.h>
#include <rnd_material.h>

namespace scn {

    class Mesh {
    public:
        Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices, res::Material textures);
        Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices);

    public:
        std::vector<res::Vertex> vertices;
        std::vector<unsigned int> indices;

        res::Material material;
    };
}