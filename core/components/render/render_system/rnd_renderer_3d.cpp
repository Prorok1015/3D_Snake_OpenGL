#if 0

#include "rnd_renderer_3d.h"
#include "rnd_render_system.h"
#include "rnd_material.h"
#include "res_resource_model.h"
#include <engine_log.h>

rnd::renderer_3d::renderer_3d(driver::driver_interface* drv_)
    :drv(drv_)
{
    vertex_array = drv->create_vertex_array();

    vertex_buffer = drv->create_buffer();
    vertex_buffer->set_data(nullptr, 20000 * sizeof(res::Vertex), rnd::driver::BUFFER_BINDING::DYNAMIC);
    vertex_buffer->set_layout(
        {
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "position"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "normal"},
            {rnd::driver::SHADER_DATA_TYPE::VEC2_F, "texture_position"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "tangent"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "bitangent"},
            {rnd::driver::SHADER_DATA_TYPE::VEC4_I,   "bones"},
            {rnd::driver::SHADER_DATA_TYPE::VEC4_F, "bones_weight"},
        }
        );

    vertex_array->add_vertex_buffer(vertex_buffer);

    std::vector<unsigned int> indices;
    indices.reserve(20000 * 6);
    uint32_t offset = 0;
    for (uint32_t i = 0; i < 20000 * 6; i += 6)
    {
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);

        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset + 0);

        offset += 4;
    }

    index_buffer = drv->create_buffer();
    index_buffer->set_data(indices.data(), indices.size() * sizeof(unsigned int), rnd::driver::BUFFER_BINDING::STATIC, rnd::driver::BUFFER_TYPE::ELEMENT_ARRAY_BUFFER);

    vertex_array->set_index_buffer(index_buffer);
}

rnd::renderer_3d::~renderer_3d()
{

}

void rnd::renderer_3d::draw(scene::Model& val)
{
    drv->set_viewport(camera->get_viewport());
    auto shader = rnd::get_system().get_shader_manager().use("scene");
    shader.uniform("model", val.model);

    for (auto& mesh : val.meshes)
    {
        draw(mesh);
    }
}

void rnd::renderer_3d::draw(scene::Mesh& mesh)
{
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    vertex_array->bind();
    vertex_buffer->set_data(mesh.vertices.data(), mesh.vertices.size() * sizeof(res::Vertex), driver::BUFFER_BINDING::DYNAMIC);

    if (mesh.indices.empty() || mesh.material.is_self_indecex) {
        vertex_array->set_index_buffer(index_buffer);
    } else {
        auto tmp = drv->create_buffer();
        tmp->set_data(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int), driver::BUFFER_BINDING::STATIC, rnd::driver::BUFFER_TYPE::ELEMENT_ARRAY_BUFFER);
        vertex_array->set_index_buffer(std::move(tmp));
    }

    drv->set_activate_texture(0);
    rnd::get_system().get_texture_manager().require_texture(mesh.material.texture_tag)->bind();

    // draw mesh
    drv->draw_indeces(rnd::get_system().get_render_mode(), (unsigned)mesh.indices.size());

    // always good practice to set everything back to defaults once configured.
    drv->set_activate_texture(0);

    vertex_array->unbind();
}
#endif