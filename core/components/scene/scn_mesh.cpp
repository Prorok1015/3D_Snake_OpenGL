#include "scn_mesh.h"
#include <rnd_render_system.h>

scene::Mesh::Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices, const res::Tag& textures)
    : Mesh(vertices, indices)
{
    material.texture = textures;
}

scene::Mesh::Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;
}

scene::Mesh::~Mesh()
{
}
