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

#include <debug_ui_api.h>
#include <Windows.h>

gs::renderer_3d::renderer_3d()
	: rnd::renderer_base(1) 
{
    start_time = GetTickCount();
    rnd::driver::driver_interface* drv = rnd::get_system().get_driver();

    vertex_array = drv->create_vertex_array();

    vertex_buffer = drv->create_buffer();
    vertex_buffer->reserve(800000 * sizeof(res::Vertex));
    vertex_buffer->set_layout(
        {
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "position"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "normal"},
            {rnd::driver::SHADER_DATA_TYPE::VEC2_F, "texture_position"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "tangent"},
            {rnd::driver::SHADER_DATA_TYPE::VEC3_F, "bitangent"},
            {rnd::driver::SHADER_DATA_TYPE::VEC4_F, "bones_weight"},
            {rnd::driver::SHADER_DATA_TYPE::VEC4_F, "color"},
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

    DBG_UI_REG_LAMBDA("RENDER/TEST_RENDER", [this]() { return is_flag_test_render; });
    DBG_UI_MENU_ITEM_CHECK_LAMBDA("RENDER/TEST_RENDER", [this](bool flag) { is_flag_test_render = flag; });
    DBG_UI_SET_ITEM_CHECKED("RENDER/TEST_RENDER", true);

    DBG_UI_REG_LAMBDA("RENDER/SHOW_ANIM", [this]() { return is_flag_show_anim; });
    DBG_UI_MENU_ITEM_CHECK_LAMBDA("RENDER/SHOW_ANIM", [this](bool flag) { is_flag_show_anim = flag; });
    DBG_UI_SET_ITEM_CHECKED("RENDER/SHOW_ANIM", true);
}

gs::renderer_3d::~renderer_3d()
{

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
    rnd::global_params common_matrix{ .time = (float)Timer::now() };

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
	    drv->set_viewport(camera->viewport);
        drv->clear(rnd::driver::CLEAR_FLAGS::COLOR_BUFFER);
        drv->clear(rnd::driver::CLEAR_FLAGS::DEPTH_BUFFER);


        draw_instances(drv);

        draw_model(drv);

        draw_sky(drv);
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
    drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
    drv->enable(rnd::driver::ENABLE_FLAGS::FACE_CULLING);

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

        rnd::shader_scene_instance_desc desc;
        if (inst->tpl.material.diffuse.is_valid()) {
            desc.tex0 = rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.diffuse)->get();
        }

        if (inst->tpl.material.specular.is_valid()) {
            desc.tex1 = rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.specular)->get();
        }

        if (inst->tpl.material.ambient.is_valid()) {
            desc.tex2 = rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.ambient)->get();
        }

        rnd::get_system().get_shader_manager().use(desc);

        if (inst->worlds.size() == 1) {
            drv->draw_indeces(vertex_array_inst, tmp, inst->tpl.indices.size());
        } else {
            drv->draw_instanced_indeces(vertex_array_inst, tmp, inst->tpl.indices.size(), inst->worlds.size());
        }
    }
}

void gs::renderer_3d::draw_model(rnd::driver::driver_interface* drv)
{
    drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
    //drv->enable(rnd::driver::ENABLE_FLAGS::FACE_CULLING);


    for (auto ent : ecs::filter<scn::is_render_component_flag, scn::model_comonent, scn::transform_component>()) {
        auto* model = ecs::get_component<scn::transform_component>(ent);
        auto* meshes = ecs::get_component<scn::model_comonent>(ent);
        
        rnd::RENDER_MODE tmp = rnd::get_system().get_render_mode();

        if (auto* rnd_mode = ecs::get_component<rnd::render_mode_component>(ent)) {
            rnd::get_system().set_render_mode(rnd_mode->mode);
        }

        if (meshes->model && is_flag_test_render) {
            rnd::shader_scene_desc scene;

            if (meshes->model->get_model_pres().animations.size() > 0 && is_flag_show_anim) {

                long long test123 = GetTickCount();
                std::vector<glm::mat4> bones = meshes->model->get_bone_transforms((float(test123 - start_time) / 1000.f), meshes->model->get_model_pres().animations[0].name);
                scene.use_animation = 1;
                rnd::bones_matrices bones_matreces;
                bones_matreces.row_height = meshes->model->get_model_pres().data.bones_data.original_size.x;
                bones_matreces.bone_count = meshes->model->get_model_pres().data.bones_data.original_size.y;
                if (bones.size() < rnd::bones_matrices::MAX_BONE_MATRICES_COUNT) {
                    std::copy(bones.begin(), bones.end(), bones_matreces.bones);
                    rnd::get_system().get_shader_manager().update_global_bones_matrices(bones_matreces, bones.size());
                } else {
                    ASSERT_FAIL("Bones matrices count too big.");
                }
            }
            else {
                scene.use_animation = 0;
            }

            scene.uWorldModel = model->world;
            res::node_hierarchy_view& hir = meshes->model->get_model_pres().head;
            auto& data = meshes->model->get_model_pres().data;
            vertex_buffer->set_data(data.vertices);
            draw_hierarchy(data, scene, model->world, hir, glm::mat4{ 1.0 }, drv);

        } else {
            rnd::shader_scene_desc scene;
            scene.uWorldModel = model->world;
            scene.uWorldMeshMatr = glm::mat4(1.0);
            scene.use_animation = 0;
            for (auto& mesh : meshes->meshes) {
                draw(scene, mesh, drv);
            }
        }

        rnd::get_system().set_render_mode(tmp);
    }
}

void gs::renderer_3d::draw_hierarchy(res::meshes_conteiner& data, rnd::shader_scene_desc& desc, glm::mat4& model_world, res::node_hierarchy_view& hir, glm::mat4 parent, rnd::driver::driver_interface* drv)
{
    int dr = 0;
    for (auto& node : hir.children)
    {
        glm::mat4 node_mt = parent * node.mt;
        desc.uWorldMeshMatr = model_world * node_mt;
        for (auto& v_mesh : node.meshes) {
            draw(desc, v_mesh, data, drv);
        }

        draw_hierarchy(data, desc, model_world, node, node_mt, drv);
    }
}

void gs::renderer_3d::draw_sky(rnd::driver::driver_interface* drv)
{
    drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST_LEQUEL);
    drv->disable(rnd::driver::ENABLE_FLAGS::FACE_CULLING);

    for (auto sky : ecs::filter<scn::is_render_component_flag, rnd::cubemap_component>()) {
        auto* cube_map = ecs::get_component<rnd::cubemap_component>(sky);
        rnd::shader_sky_desc sky;
        sky.tex0 = rnd::get_system().get_texture_manager().require_cubemap_texture(cube_map->cube_map)->get();
        auto& vs = cube_map->mesh.vertices;
        auto& is = cube_map->mesh.indices;
        vertex_buffer->set_data(vs);
        index_buffer->set_data(is);

        rnd::get_system().get_shader_manager().use(sky);
        drv->draw_indeces(vertex_array, rnd::RENDER_MODE::TRIANGLE, is.size());
    }
}

void gs::renderer_3d::draw(scn::Model& val, rnd::driver::driver_interface* drv)
{
    rnd::shader_scene_desc scene;
    scene.uWorldModel = val.model;
	for (auto& mesh : val.meshes) {
		draw(scene, mesh, drv);
	}
}

void gs::renderer_3d::draw(rnd::shader_scene_desc& desc, res::Mesh& mesh, rnd::driver::driver_interface* drv)
{
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    vertex_buffer->set_data(mesh.vertices);
    index_buffer->set_data(mesh.indices);

    if (mesh.material.diffuse.is_valid()) {
        desc.tex0 = rnd::get_system().get_texture_manager().require_texture(mesh.material.diffuse)->get();
    }

    if (mesh.material.specular.is_valid()) {
        desc.tex1 = rnd::get_system().get_texture_manager().require_texture(mesh.material.specular)->get();
    }

    if (mesh.material.ambient.is_valid()) {
        desc.tex2 = rnd::get_system().get_texture_manager().require_texture(mesh.material.ambient)->get();
    }

    rnd::get_system().get_shader_manager().use(desc);
    // draw mesh
    drv->draw_indeces(vertex_array, rnd::get_system().get_render_mode(), (unsigned)mesh.indices.size());
}


void gs::renderer_3d::draw(rnd::shader_scene_desc& desc, res::mesh_view& mesh, res::meshes_conteiner& data, rnd::driver::driver_interface* drv)
{
    index_buffer->set_data_ptr(&data.indices[mesh.ind_begin], mesh.get_indices_count());

    auto& material = data.materials[mesh.material_id];
    if (material.diffuse.is_valid()) {
        desc.tex0 = rnd::get_system().get_texture_manager().require_texture(material.diffuse)->get();
    }

    if (material.specular.is_valid()) {
        desc.tex1 = rnd::get_system().get_texture_manager().require_texture(material.specular)->get();
    }

    if (material.ambient.is_valid()) {
        desc.tex2 = rnd::get_system().get_texture_manager().require_texture(material.ambient)->get();
    }

    if (data.bones_data.bones_indeces_txm.is_valid()) {
        desc.tex3 = rnd::get_system().get_texture_manager().require_texture(data.bones_data.bones_indeces_txm)->get();
    }

    rnd::get_system().get_shader_manager().use(desc);
    // draw mesh
    drv->draw_indeces(vertex_array, rnd::get_system().get_render_mode(), mesh.get_indices_count(), mesh.vx_begin);
}
