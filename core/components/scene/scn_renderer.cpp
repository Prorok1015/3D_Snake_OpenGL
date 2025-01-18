#include "scn_renderer.h"
#include "rnd_driver_interface.h"
#include "rnd_vertex_array_interface.h"
#include "rnd_buffer_interface.h"
#include "rnd_render_system.h"
#include "logger/engine_log.h"
#include "scn_primitives.h"

#include "res_instance.h"

#include "ecs_common_system.h"

#include "timer.hpp"
#include "eng_transform_3d.hpp"

#include "gui_api.hpp"

namespace scn {
    float make_aspect(camera_component& camera)
    {
        return (float)camera.viewport.size.x / (float)camera.viewport.size.y;
    }

    glm::mat4 make_projection(camera_component& camera)
    {
        return glm::perspective(glm::radians(camera.fov), make_aspect(camera), camera.near_distance, camera.far_distance);
    }
}

scn::renderer_3d::renderer_3d()
    : rnd::renderer_base(1)
{
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

scn::renderer_3d::~renderer_3d()
{

}

void scn::renderer_3d::setup_instance_buffer()
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

void scn::renderer_3d::on_render(rnd::driver::driver_interface* drv)
{
    rnd::global_params common_matrix{ .time = (float)Timer::now() };

    static res::Tag color_rt_tag = res::Tag(res::Tag::memory, "__color_scene_rt");
    auto& txm_manager = rnd::get_system().get_texture_manager();
    for (ecs::entity& ent : ecs::filter<scn::light_point>()) {
        auto* light = ecs::get_component<scn::light_point>(ent);

        rnd::sun_params sun;
        sun.ambient = light->ambient;
        sun.diffuse = light->diffuse;
        sun.specular = light->specular;
        sun.position = light->position;
        rnd::get_system().get_shader_manager().update_global_sun(sun);
    }
    static bool first_init = true;
    for (ecs::entity& ent : ecs::filter<scn::camera_component, scn::is_render_component_flag>())
    {
        scn::camera_component* camera = ecs::get_component<scn::camera_component>(ent);
        if (camera->viewport.size.x < 1 || camera->viewport.size.y < 1) {
            continue;
        }
        auto color_rt = txm_manager.find(color_rt_tag);
        if (!color_rt) {
            rnd::driver::texture_header header;
            header.picture.height = camera->viewport.size.y;
            header.picture.width = camera->viewport.size.x;
            header.picture.channels = rnd::driver::texture_header::TYPE::RGBA8;
            header.wrap = rnd::driver::texture_header::WRAPPING::CLAMP_TO_EDGE;
            header.mag = rnd::driver::texture_header::FILTERING::NEAREST;
            header.min = rnd::driver::texture_header::FILTERING::NEAREST;
            color_rt = txm_manager.generate_texture(color_rt_tag, header);
            egLOG("scn/renderer", "'{2}' render target Created size: {0}, {1}", color_rt->width(), color_rt->height(), color_rt_tag.name());
        }
        else if (color_rt->width() != camera->viewport.size.x || color_rt->height() != camera->viewport.size.y)
        {
            txm_manager.remove(color_rt_tag);

            rnd::driver::texture_header header;
            header.picture.height = camera->viewport.size.y;
            header.picture.width = camera->viewport.size.x;
            header.picture.channels = rnd::driver::texture_header::TYPE::RGBA8;
            header.wrap = rnd::driver::texture_header::WRAPPING::CLAMP_TO_EDGE;
            header.mag = rnd::driver::texture_header::FILTERING::NEAREST;
            header.min = rnd::driver::texture_header::FILTERING::NEAREST;
            color_rt = txm_manager.generate_texture(color_rt_tag, header);
            egLOG("scn/renderer", "'{2}' render target Recreated size: {0}, {1}", color_rt->width(), color_rt->height(), color_rt_tag.name());
        }

        drv->push_frame_buffer();
        drv->set_render_rarget(color_rt);
        drv->clear(rnd::driver::CLEAR_FLAGS::COLOR_BUFFER);
        drv->clear(rnd::driver::CLEAR_FLAGS::DEPTH_BUFFER);

        eng::transform3d pos{ glm::mat4{1.0} };
        common_matrix.view = glm::inverse(glm::mat4{ 1.0 });

        if (auto* trans = ecs::get_component<scn::transform_component>(ent))
        {
            common_matrix.view = glm::inverse(trans->local);
            pos = eng::transform3d{ trans->local };
        }

        common_matrix.projection = scn::make_projection(*camera);
        common_matrix.view_position = glm::vec4(pos.get_pos(), 1.0);

        rnd::get_system().get_shader_manager().update_global_uniform(common_matrix);
        glm::ivec4 vp{ camera->viewport.center, camera->viewport.size };
        drv->set_viewport(vp);


        draw_instances(drv);

        draw_ecs_model(drv);

        draw_sky(drv);

        drv->pop_frame_buffer();
    }
}

void scn::renderer_3d::draw_instances(rnd::driver::driver_interface* drv)
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
            desc.tex0 = rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.diffuse);
        }

        if (inst->tpl.material.specular.is_valid()) {
            desc.tex1 = rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.specular);
        }

        if (inst->tpl.material.ambient.is_valid()) {
            desc.tex2 = rnd::get_system().get_texture_manager().require_texture(inst->tpl.material.ambient);
        }

        rnd::get_system().get_shader_manager().use(desc);

        if (inst->worlds.size() == 1) {
            drv->draw_indeces(vertex_array_inst, tmp, inst->tpl.indices.size());
        }
        else {
            drv->draw_instanced_indeces(vertex_array_inst, tmp, inst->tpl.indices.size(), inst->worlds.size());
        }
    }
}

void scn::renderer_3d::draw_ecs_meshes(ecs::entity ent, const res::meshes_conteiner& data, rnd::shader_scene_desc& scene, rnd::driver::driver_interface* drv)
{
    if (auto* meshes = ecs::get_component<scn::mesh_component>(ent))
    {
        scene.uWorldMeshMatr = glm::mat4{ 1.0 };
        if (auto* transform = ecs::get_component<scn::transform_component>(ent))
        {
            scene.uWorldMeshMatr = transform->world;
        }
        draw(scene, meshes->mesh, data, drv);
    }

    if (auto* children = ecs::get_component<scn::children_component>(ent))
    {
        for (auto& child : children->children)
        {
            draw_ecs_meshes(child, data, scene, drv);
        }
    }
}

void scn::renderer_3d::draw_ecs_model(rnd::driver::driver_interface* drv)
{
    drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
    //drv->enable(rnd::driver::ENABLE_FLAGS::FACE_CULLING);

    for (auto ent : ecs::filter<scn::model_root_component, scn::is_render_component_flag>()) {
        auto* transform = ecs::get_component<scn::transform_component>(ent);
        auto* root = ecs::get_component<scn::model_root_component>(ent);
        rnd::RENDER_MODE tmp = rnd::get_system().get_render_mode();

        if (auto* rnd_mode = ecs::get_component<rnd::render_mode_component>(ent)) {
            rnd::get_system().set_render_mode(rnd_mode->mode);
        }

        vertex_buffer->set_data(root->data.vertices);
        rnd::shader_scene_desc scene;
        if (auto* anim = ecs::get_component<scn::playable_animation>(ent)) {
            scene.use_animation = 1;
            rnd::bones_matrices bones_matreces;
            bones_matreces.row_height = root->data.bones_data.original_size.x;
            bones_matreces.bone_count = root->data.bones_data.original_size.y;
            auto& bones = root->data.bones_matrices;
            if (bones.size() < rnd::bones_matrices::MAX_BONE_MATRICES_COUNT) {
                std::copy(bones.begin(), bones.end(), bones_matreces.bones);
                rnd::get_system().get_shader_manager().update_global_bones_matrices(bones_matreces, bones.size());
            }
            else {
                ASSERT_FAIL("Bones matrices count too big.");
            }        
        } else {
            scene.use_animation = 0;
        }

        draw_ecs_meshes(ent, root->data, scene, drv);

        rnd::get_system().set_render_mode(tmp);
    }
}

void scn::renderer_3d::draw_sky(rnd::driver::driver_interface* drv)
{
    drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST_LEQUEL);
    drv->disable(rnd::driver::ENABLE_FLAGS::FACE_CULLING);

    for (auto sky : ecs::filter<scn::is_render_component_flag, scn::sky_component>()) {
        auto* cube_map = ecs::get_component<scn::sky_component>(sky);
        rnd::shader_sky_desc sky;
        sky.tex0 = rnd::get_system().get_texture_manager().require_cubemap_texture(cube_map->cube_map);
        auto& vs = cube_map->data.vertices;
        auto& is = cube_map->data.indices;
        vertex_buffer->set_data(vs);
        index_buffer->set_data(is);

        rnd::configure_pass(sky);
        drv->draw_indeces(vertex_array, rnd::RENDER_MODE::TRIANGLE, is.size());
    }
}

void scn::renderer_3d::draw(rnd::shader_scene_desc& desc, res::mesh_view& mesh, const res::meshes_conteiner& data, rnd::driver::driver_interface* drv)
{
    index_buffer->set_data_ptr(&data.indices[mesh.ind_begin], mesh.get_indices_count());

    auto& material = data.materials[mesh.material_id];
    if (material.diffuse.is_valid()) {
        desc.tex0 = rnd::get_system().get_texture_manager().require_texture(material.diffuse);
    }

    if (material.specular.is_valid()) {
        desc.tex1 = rnd::get_system().get_texture_manager().require_texture(material.specular);
    }

    if (material.ambient.is_valid()) {
        desc.tex2 = rnd::get_system().get_texture_manager().require_texture(material.ambient);
    }

    if (data.bones_data.bones_indeces_txm.is_valid()) {
        desc.tex3 = rnd::get_system().get_texture_manager().require_texture(data.bones_data.bones_indeces_txm);
    }

    rnd::configure_pass(desc);
    // draw mesh
    drv->draw_indeces(vertex_array, rnd::get_system().get_render_mode(), mesh.get_indices_count(), mesh.vx_begin);
}
