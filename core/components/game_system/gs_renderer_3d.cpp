#include "gs_renderer_3d.h"
#include <rnd_driver_interface.h>
#include <rnd_vertex_array_interface.h>
#include <rnd_buffer_interface.h>
#include <rnd_render_system.h>

#include <scn_primitives.h>

#include <res_instance.h>

#include <light/rnd_light_point.h>
#include <sky/rnd_cubemap.h>

#include <ecs/ecs_common_system.h>

#include <timer.hpp>

gs::renderer_3d::renderer_3d()
	: rnd::renderer_base(1) 
{
    rnd::driver::driver_interface* drv = rnd::get_system().get_driver();

    vertex_array = drv->create_vertex_array();

    vertex_buffer = drv->create_buffer();
    vertex_buffer->set_data(nullptr, 800000 * sizeof(res::Vertex), rnd::driver::BUFFER_BINDING::DYNAMIC);
    vertex_buffer->set_layout(
        {
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "position"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "normal"},
            {rnd::driver::SHADER_DATA_TYPE::VEC2_F, "texture_position"},/*
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "tangent"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "bitangent"},
            {rnd::driver::SHADER_DATA_TYPE::VEC4_I, "bones"},
            {rnd::driver::SHADER_DATA_TYPE::VEC4_F, "bones_weight"},*/
        }
    );

    vertex_array->add_vertex_buffer(vertex_buffer);

    std::vector<unsigned int> indices;
    indices.reserve(800000 * 6);
    uint32_t offset = 0;
    for (uint32_t i = 0; i < 800000 * 6; i += 6)
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
    index_buffer->set_data(indices);

    vertex_array->set_index_buffer(index_buffer);

    setup_instance_buffer();
}

void gs::renderer_3d::setup_instance_buffer()
{
    rnd::driver::driver_interface* drv = rnd::get_system().get_driver();
    vertex_array_inst = drv->create_vertex_array();

    //vertex_buffer_inst = drv->create_buffer();
    //vertex_buffer_inst->set_data(nullptr, 800000 * sizeof(res::Vertex), rnd::driver::BUFFER_BINDING::DYNAMIC);
    /*vertex_buffer_inst->set_layout(
        {
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "position"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "normal"},
            {rnd::driver::SHADER_DATA_TYPE::VEC2_F, "texture_position"}
        }
        );*/
    vertex_array_inst->add_vertex_buffer(vertex_buffer);
    matrices_buffer_inst = drv->create_buffer();
    matrices_buffer_inst->set_data(nullptr, sizeof(glm::mat4) * 3000, rnd::driver::BUFFER_BINDING::DYNAMIC);
    matrices_buffer_inst->set_layout({ {rnd::driver::SHADER_DATA_TYPE::MAT4_F, "world"} });
    vertex_array_inst->add_vertex_buffer(matrices_buffer_inst);

    //index_buffer_inst = drv->create_buffer();
    //index_buffer_inst->set_data(nullptr, 800000 * 6, rnd::driver::BUFFER_BINDING::DYNAMIC);
    vertex_array_inst->set_index_buffer(index_buffer);
}

void gs::renderer_3d::on_render(rnd::driver::driver_interface* drv)
{

    rnd::GlobalUniform common_matrix{ .time = (float)Timer::now() };

    for (ecs::entity& ent : ecs::filter<rnd::light_point>()) {
        auto* light = ecs::get_component<rnd::light_point>(ent);

        rnd::get_system().get_shader_manager().update_global_sun(*light);
    }

    for (ecs::entity& ent : ecs::filter<rnd::camera_component, scn::is_render_component_flag>())
    {
        rnd::camera_component* camera = ecs::get_component<rnd::camera_component>(ent);
        if (camera->viewport[2] == 0 || camera->viewport[3] == 0) {
            continue;
        }

        common_matrix.projection = rnd::make_projection(*camera);
        common_matrix.view = rnd::make_view(*camera);
        common_matrix.view_position = glm::vec4(eng::transform3d(camera->world).get_pos(), 1.0);

        rnd::get_system().get_shader_manager().update_global_uniform(common_matrix);

        drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
        drv->enable(rnd::driver::ENABLE_FLAGS::FACE_CULLING);

        drv->clear(rnd::driver::CLEAR_FLAGS::COLOR_BUFFER);
        drv->clear(rnd::driver::CLEAR_FLAGS::DEPTH_BUFFER);

	    drv->set_viewport(camera->viewport);

        draw_instances(drv);

        auto shader = rnd::get_system().get_shader_manager().use("scene");

        for (auto ent : ecs::filter<scn::is_render_component_flag, scn::model_comonent, scn::transform_component>()) {
            auto* model = ecs::get_component<scn::transform_component>(ent);
            shader.uniform("model", model->world);

            shader.uniform("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
            shader.uniform("material.shininess", 32.0f);

            rnd::RENDER_MODE tmp = rnd::get_system().get_render_mode();

            if (auto* rnd_mode = ecs::get_component<rnd::render_mode_component>(ent)) {
                rnd::get_system().set_render_mode(rnd_mode->mode);
            }

            auto* meshes = ecs::get_component<scn::model_comonent>(ent);
            for (auto& mesh : meshes->meshes) {
                draw(mesh, drv);
            }

            rnd::get_system().set_render_mode(tmp);
        }

        drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST_LEQUEL);
        drv->disable(rnd::driver::ENABLE_FLAGS::FACE_CULLING);

        for (auto sky : ecs::filter<scn::is_render_component_flag, rnd::cubemap_component>()) {
            auto* cube_map = ecs::get_component<rnd::cubemap_component>(sky);
            auto shader = rnd::get_system().get_shader_manager().use("sky");

            auto vs = cube_map->mesh.vertices;
            auto is = cube_map->mesh.indices;
            vertex_buffer->set_data(vs.data(),vs.size() * sizeof(res::Vertex), rnd::driver::BUFFER_BINDING::DYNAMIC);
            index_buffer->set_data(is.data(), is.size() * sizeof(unsigned int), rnd::driver::BUFFER_BINDING::DYNAMIC);

            rnd::get_system().get_texture_manager().require_cubemap_texture(cube_map->cube_map)->bind(0);

            vertex_array->bind();
            drv->draw_indeces(rnd::RENDER_MODE::TRIANGLE, is.size());
            vertex_array->unbind();
        }
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
    //drv->draw_indeces(rnd::RENDER_MODE::LINE, 2);

    //// always good practice to set everything back to defaults once configured.
    //drv->set_activate_texture(0);

    //vertex_array->unbind();
}

void gs::renderer_3d::draw_instances(rnd::driver::driver_interface* drv)
{
    auto shader = rnd::get_system().get_shader_manager().use("scene_inst");

    for (auto ent : ecs::filter<res::instance_object>()) {
        res::instance_object* inst = ecs::get_component<res::instance_object>(ent);
        if (inst->worlds.empty()) {
            continue;
        }

        rnd::RENDER_MODE tmp = rnd::get_system().get_render_mode();

        if (auto* rnd_mode = ecs::get_component<rnd::render_mode_component>(ent)) {
            tmp = rnd_mode->mode;
        }
        
        vertex_buffer->set_data(inst->tpl.vertices);
        index_buffer->set_data(inst->tpl.indices);

        matrices_buffer_inst->set_data(inst->worlds);

        if (inst->tpl.material.diffuse.is_valid()) {
            rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.diffuse)->bind(0);
        }

        if (inst->tpl.material.specular.is_valid()) {
            rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.specular)->bind(1);
        }

        if (inst->tpl.material.ambient.is_valid()) {
            rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.ambient)->bind(2);
        }

        vertex_array_inst->bind();
        if (inst->worlds.size() == 1) {
            drv->draw_indeces(tmp, inst->tpl.indices.size());
        }
        else {
            drv->draw_instanced_indeces(tmp, inst->tpl.indices.size(), inst->worlds.size());
        }
        vertex_array_inst->unbind();
    }
}

void gs::renderer_3d::draw(scn::Model& val, rnd::driver::driver_interface* drv)
{
	auto shader = rnd::get_system().get_shader_manager().use("scene");
	shader.uniform("model", val.model);

	for (auto& mesh : val.meshes) {
		draw(mesh, drv);
	}
}

void gs::renderer_3d::draw(res::Mesh& mesh, rnd::driver::driver_interface* drv)
{
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    vertex_buffer->set_data(mesh.vertices);
    index_buffer->set_data(mesh.indices);

    if (mesh.material.diffuse.is_valid()) {
        rnd::get_system().get_texture_manager().require_texture(mesh.material.diffuse)->bind(0);
    }

    if (mesh.material.specular.is_valid()) {
        rnd::get_system().get_texture_manager().require_texture(mesh.material.specular)->bind(1);
    }

    if (mesh.material.ambient.is_valid()) {
        rnd::get_system().get_texture_manager().require_texture(mesh.material.ambient)->bind(2);
    }

    // draw mesh
    vertex_array->bind();
    drv->draw_indeces(rnd::get_system().get_render_mode(), (unsigned)mesh.indices.size());
    vertex_array->unbind();
}

