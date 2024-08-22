#include "gs_renderer_3d.h"
#include <rnd_driver_interface.h>
#include <rnd_vertex_array_interface.h>
#include <rnd_buffer_interface.h>
#include <rnd_render_system.h>

#include <timer.hpp>

gs::renderer_3d::renderer_3d()
	: rnd::renderer_base(1) 
{
    rnd::driver::driver_interface* drv = rnd::get_system().get_driver();

    vertex_array = drv->create_vertex_array();

    vertex_buffer = drv->create_buffer();
    vertex_buffer->set_data(nullptr, 20000 * sizeof(res::Vertex), rnd::driver::BUFFER_BINDING::DYNAMIC);
    vertex_buffer->set_layout(
        {
            {rnd::driver::ShaderDataType::Float3, "position"},
            {rnd::driver::ShaderDataType::Float3, "normal"},
            {rnd::driver::ShaderDataType::Float2, "texture_position"},
            {rnd::driver::ShaderDataType::Float3, "tangent"},
            {rnd::driver::ShaderDataType::Float3, "bitangent"},
            {rnd::driver::ShaderDataType::Int4,   "bones"},
            {rnd::driver::ShaderDataType::Float4, "bones_weight"},
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

    index_buffer = drv->create_buffer();
    index_buffer->set_data(indeces.data(), indeces.size() * sizeof(unsigned int), rnd::driver::BUFFER_BINDING::STATIC, rnd::driver::BUFFER_TYPE::ELEMENT_ARRAY_BUFFER);

    vertex_array->set_index_buffer(index_buffer);
}

void gs::renderer_3d::on_render(rnd::driver::driver_interface* drv)
{
	if (!camera || (camera && camera->get_viewport() == glm::zero<glm::ivec4>())) {
		return;
	}

    drv->clear(rnd::driver::CLEAR_FLAGS::COLOR_BUFFER);
    drv->clear(rnd::driver::CLEAR_FLAGS::DEPTH_BUFFER);

    rnd::GlobalUniform val
    {
        .projection = camera->projection(),
        .view = camera->view(),
        .time = (float)Timer::now()
    };

    rnd::get_system().get_shader_manager().update_global_uniform(val);

	drv->set_viewport(camera->get_viewport());

	// render
	for (auto& model : scene_objects) {
		draw(model, drv);
	}

}

void gs::renderer_3d::draw_line(rnd::driver::driver_interface* drv)
{
    //auto shader = rnd::get_system().get_shader_manager().use("scene");
    //glm::mat4 lineModel = camera->transform.to_matrix();
    //shader.uniform("model", /*lineModel * */glm::scale(glm::translate(glm::mat4{ 1 }, glm::vec3{ 0 }), glm::vec3{ 1 }));

    //res::Vertex line[2];
    //line[0].position_ = { 0.f, 0.f, 0.f };
    //line[1].position_ = { 0.f, 50.f, 0.f };

    //line[1].position_ = camera->transform.get_pos();
    ////line[1].position_ = /*camera->transform.get_pos() + */(camera->transform.back() * 50.f);

    //vertex_array->bind();
    //vertex_buffer->set_data(line, 2 * sizeof(res::Vertex), rnd::driver::BUFFER_BINDING::DYNAMIC);
    //vertex_array->set_index_buffer(index_buffer);

    //drv->set_activate_texture(0);
    //rnd::get_system().get_texture_manager().require_texture(res::Tag::make("__black"))->bind();

    //// draw mesh
    //drv->draw_elements(rnd::RENDER_MODE::LINE, 2);

    //// always good practice to set everything back to defaults once configured.
    //drv->set_activate_texture(0);

    //vertex_array->unbind();
}

void gs::renderer_3d::draw(scene::Model& val, rnd::driver::driver_interface* drv)
{
	auto shader = rnd::get_system().get_shader_manager().use("scene");
	shader.uniform("model", val.model);

	for (auto& mesh : val.meshes) {
		draw(mesh, drv);
	}
}

void gs::renderer_3d::draw(scene::Mesh& mesh, rnd::driver::driver_interface* drv)
{
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    vertex_array->bind();
    vertex_buffer->set_data(mesh.vertices.data(), mesh.vertices.size() * sizeof(res::Vertex), rnd::driver::BUFFER_BINDING::DYNAMIC);

    if (mesh.indices.empty() || mesh.material.is_self_indecex) {
        vertex_array->set_index_buffer(index_buffer);
    }
    else {
        auto tmp = drv->create_buffer();
        tmp->set_data(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int), rnd::driver::BUFFER_BINDING::STATIC, rnd::driver::BUFFER_TYPE::ELEMENT_ARRAY_BUFFER);
        vertex_array->set_index_buffer(std::move(tmp));
    }

    drv->set_activate_texture(0);
    rnd::get_system().get_texture_manager().require_texture(mesh.material.texture_tag)->bind();

    // draw mesh
    drv->draw_elements(rnd::get_system().get_render_mode(), (unsigned)mesh.indices.size());

    // always good practice to set everything back to defaults once configured.
    drv->set_activate_texture(0);

    vertex_array->unbind();
}