#include "scn_cude.h"
#include <common.h>
#include <shader/rnd_shader.h>
#include <rnd_render_system.h>
#include <numeric>
#include "scn_mesh.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
};


void init_cude()
{
    std::vector<res::Vertex> vex;
    std::vector<unsigned int> inc;
    std::vector<std::shared_ptr<rnd::Texture>> tex;

    for (int i = 0; i < 180; i += 5)
    {
        res::Vertex v;
        v.position_ = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        v.tex_uv_ = glm::vec2(vertices[i + 3] / 16, -vertices[i + 4] / 16);
        vex.push_back(v);
    }

    for (int i = 0; i < vex.size(); i += 3) {
        auto& p1 = vex[i];
        auto& p2 = vex[i + 1];
        auto& p3 = vex[i + 2];

        p1.normal_ = glm::normalize(glm::cross(p3.position_ - p2.position_, p1.position_ - p2.position_));
        p2.normal_ = p1.normal_;
        p3.normal_ = p1.normal_;
    }

    inc.resize(vex.size());
    std::iota(inc.begin(), inc.end(), 0);
    auto txt = rnd::get_system().get_txr_manager().require_texture(res::Tag::make("block.png"));
    txt->tmp_type = "texture_diffuse";
    tex.push_back(txt);
}

void draw_cude(const render::Shader& ourShader)
{

}

void term_cude()
{
}

scene::Model create_cube()
{
    std::vector<res::Vertex> vex;
    std::vector<unsigned int> inc;
    std::vector<std::shared_ptr<rnd::Texture>> tex;

    for (int i = 0; i < 180; i += 5)
    {
        res::Vertex v;
        v.position_ = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        v.tex_uv_ = glm::vec2(vertices[i + 3] / 16, -vertices[i + 4] / 16);
        vex.push_back(v);
    }

    for (int i = 0; i < vex.size(); i += 3) {
        auto& p1 = vex[i];
        auto& p2 = vex[i + 1];
        auto& p3 = vex[i + 2];

        p1.normal_ = glm::normalize(glm::cross(p3.position_ - p2.position_, p1.position_ - p2.position_));
        p2.normal_ = p1.normal_;
        p3.normal_ = p1.normal_;
    }

    inc.resize(vex.size());
    std::iota(inc.begin(), inc.end(), 0);
    //auto txt = rnd::get_system().get_txr_manager().require_texture(res::Tag::make("block.png"));
    //txt->tmp_type = "texture_diffuse";
    //tex.push_back(txt);

    scene::Model m;
    m.meshes.emplace_back(vex, inc, res::Tag::make("block.png"));
    return m;
}
