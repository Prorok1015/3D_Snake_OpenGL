#include "rnd_renderer_3d.h"
#include "rnd_render_system.h"
#include "rnd_material.h"
#include "res_resource_model.h"
//TODO
#include <rnd_gl_buffer_layout.h>

render::Renderer3d::Renderer3d()
{

}

render::Renderer3d::~Renderer3d()
{

}

void render::Renderer3d::init()
{    
    vertex_array = std::make_shared<render::driver::gl::vertex_array>();
    vertex_buffer = std::make_shared<render::driver::gl::vertex_buffer>(20000 * sizeof(res::Vertex));
    vertex_buffer->set_layout(
        {
            {render::driver::gl::ShaderDataType::Float3, "position"},
            {render::driver::gl::ShaderDataType::Float3, "normal"},
            {render::driver::gl::ShaderDataType::Float2, "texture_position"},
            {render::driver::gl::ShaderDataType::Float3, "tangent"},
            {render::driver::gl::ShaderDataType::Float3, "bitangent"},
            {render::driver::gl::ShaderDataType::Int4,   "bones"},
            {render::driver::gl::ShaderDataType::Float4, "bones_weight"},
        }
    );

    vertex_array->add_vertex_buffer(vertex_buffer);

    std::vector<unsigned int> indeces;
    indeces.reserve(20000 * 6);
    uint32_t offset = 0;
    for (uint32_t i = 0; i < 20000 * 6; i += 6)
    {
        indeces.push_back(offset + 0);
        indeces.push_back(offset + 1);
        indeces.push_back(offset + 2);

        indeces.push_back(offset + 2);
        indeces.push_back(offset + 3);
        indeces.push_back(offset + 0);

        offset += 4;
    }

    index_buffer = std::make_shared<render::driver::gl::index_buffer>(indeces);
    vertex_array->set_index_buffer(index_buffer);

}

void render::Renderer3d::term()
{

}

void render::Renderer3d::draw(scene::Model& val)
{
     auto shader = rnd::get_system().get_sh_manager().use("scene");
     shader.uniform("model", val.model);

    for (auto& mesh : val.meshes)
    {
        draw(mesh);
    }
}

void render::Renderer3d::draw(scene::Mesh& mesh)
{
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    vertex_array->bind();
    vertex_buffer->set_data(mesh.vertices);

    if (mesh.indices.empty() || mesh.material.is_self_indecex) {
        vertex_array->set_index_buffer(index_buffer);
    }
    else {
        vertex_array->set_index_buffer(std::make_shared<render::driver::gl::index_buffer>(mesh.indices.data(), mesh.indices.size()));
    }

    rnd::get_system().activate_texture_unit(0);
    rnd::get_system().get_txr_manager().require_texture(mesh.material.texture)->bind();

    // draw mesh
    rnd::get_system().draw_elements((unsigned)mesh.indices.size());

    // always good practice to set everything back to defaults once configured.
    rnd::get_system().reset_texture();

    vertex_array->unbind();
}
