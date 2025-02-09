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
#include "scn_material_component.hpp"

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
    vertex_buffer->reserve(8000000 * sizeof(res::Vertex));
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
    static res::Tag color_rt_tag = res::Tag(res::Tag::memory, "__color_scene_rt");
    static res::Tag z_pass_tag = res::Tag(res::Tag::memory, "__z_prepass_rt");
    static res::Tag color_rt_transparent_tag = res::Tag(res::Tag::memory, "__color_rt_transparent_rt");
    static res::Tag waight_rt_transparent_tag = res::Tag(res::Tag::memory, "__waight_rt_transparent_rt");
    rnd::global_params common_matrix{ .time = (float)Timer::now() };

    z_prepass(drv);

    prepare_directional_light();//TODO: move to ecs systems

    auto& txm_manager = rnd::get_system().get_texture_manager();

    for (ecs::entity& ent : ecs::filter<scn::camera_component, scn::is_render_component_flag>())
    {
        scn::camera_component* camera = ecs::get_component<scn::camera_component>(ent);
        if (camera->viewport.size.x < 1 || camera->viewport.size.y < 1) {
            continue;
        }
        auto color_rt = txm_manager.find(color_rt_tag);
        auto color_tp_rt = txm_manager.find(color_rt_transparent_tag);
        auto waight_tp_rt = txm_manager.find(waight_rt_transparent_tag);
        if (color_rt && (color_rt->size() != camera->viewport.size))
        {
            txm_manager.remove(color_rt_tag);
            txm_manager.remove(color_rt_transparent_tag);
            txm_manager.remove(waight_rt_transparent_tag);
            color_rt = nullptr;
            color_tp_rt = nullptr;
            waight_tp_rt = nullptr;
        }

        if (!color_rt) {
            rnd::driver::texture_header header;
            header.picture.height = camera->viewport.size.y;
            header.picture.width = camera->viewport.size.x;
            header.picture.channels = rnd::driver::texture_header::TYPE::RGBA8;
            header.wrap = rnd::driver::texture_header::WRAPPING::CLAMP_TO_EDGE;
            header.mag = rnd::driver::texture_header::FILTERING::NEAREST;
            header.min = rnd::driver::texture_header::FILTERING::NEAREST;
            color_rt = txm_manager.generate_texture(color_rt_tag, header);

            header.picture.channels = rnd::driver::texture_header::TYPE::RGBA16F;
            header.wrap = rnd::driver::texture_header::WRAPPING::CLAMP_TO_EDGE;
            header.mag = rnd::driver::texture_header::FILTERING::LINEAR;
            header.min = rnd::driver::texture_header::FILTERING::LINEAR;
            color_tp_rt = txm_manager.generate_texture(color_rt_transparent_tag, header);
            egLOG("scn/renderer", "'{2}' render target Created size: {0}, {1}", color_rt->width(), color_rt->height(), color_rt_tag.name());
            header.picture.channels = rnd::driver::texture_header::TYPE::R8;
            waight_tp_rt = txm_manager.generate_texture(waight_rt_transparent_tag, header);
        }

        {
            // Main view NON-transparent objects
            drv->disable(rnd::driver::ENABLE_FLAGS::DEPTH_MASK);
            drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST_LEQUEL);
            drv->push_frame_buffer();
            drv->set_render_rarget(color_rt, txm_manager.find(z_pass_tag));
            drv->clear(rnd::driver::CLEAR_FLAGS::COLOR_BUFFER, {glm::vec4(0)});

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

            drv->set_viewport(camera->viewport);
            draw_instances(drv);
            rnd::shader_scene_desc scene{};
            draw_ecs_model(drv, scene);
            draw_sky(drv);
            drv->pop_frame_buffer();
        }

        {
            // Main view TRANSPARENT objects
            // Order-Independent Transparency (OIT)        
            drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
            drv->disable(rnd::driver::ENABLE_FLAGS::DEPTH_MASK);

            drv->push_frame_buffer();
            drv->set_render_rargets({ color_tp_rt, waight_tp_rt }, txm_manager.find(z_pass_tag));
            //BLAND MODE ONE ONE
            drv->enable(rnd::driver::ENABLE_FLAGS::BLEND_MODE_ADD, 0);
            drv->enable(rnd::driver::ENABLE_FLAGS::BLEND_MODE_ZERO_MINUS_ALPHA, 1);
            drv->clear(rnd::driver::CLEAR_FLAGS::COLOR_BUFFER, { glm::vec4(0), glm::vec4(1) });

            drv->set_viewport(camera->viewport);
            draw_transparent(drv);

            drv->pop_frame_buffer();

            drv->disable(rnd::driver::ENABLE_FLAGS::BLEND_MODE_ADD);
        }
        {
            // Composition
            drv->enable(rnd::driver::ENABLE_FLAGS::BLEND_MODE_MINUS_ALPHA, 0);
            drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST_ALWAYS);

            drv->push_frame_buffer();
            drv->set_render_rarget(color_rt);

            drv->set_viewport(camera->viewport);
            draw_composition(drv, color_tp_rt, waight_tp_rt);

            drv->pop_frame_buffer();

            drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
            drv->disable(rnd::driver::ENABLE_FLAGS::BLEND_MODE_MINUS_ALPHA);
        }
    }
}

void scn::renderer_3d::prepare_directional_light()
{
    rnd::lights_params global_lights;
    directional_light_count = 0;
    for (ecs::entity& ent : ecs::filter<scn::directional_light>()) {
        if (directional_light_count >= rnd::lights_params::MAX_LIGHT_COUNT) {
            ASSERT_FAIL("Maxsimum of Directional lights is 12");
            break;
        }
        auto* light = ecs::get_component<scn::directional_light>(ent);

        rnd::lights_params::directional_light sun;
        sun.ambient = light->ambient;
        sun.diffuse = light->diffuse;
        sun.specular = light->specular;
        sun.direction = light->direction;
        global_lights.directional_lights[directional_light_count++] = sun;
    }
    rnd::get_system().get_shader_manager().update_global_sun(global_lights);
}

void scn::renderer_3d::draw_instances(rnd::driver::driver_interface* drv)
{
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
        auto& material = inst->tpl.material;
        if (material.is_state(res::Material::ALBEDO_TXM)) {
            desc.tex0 = rnd::get_system().get_texture_manager().require_texture(material.get_txm(res::Material::ALBEDO_TXM));
            desc.defines[rnd::shader_scene_desc::USE_TXM_AS_DIFFUSE] = true;
        }
        else if (material.is_state(res::Material::ALBEDO_COLOR)) {
            //desc.diffuseColor = material.diffuse_color;
            desc.defines[rnd::shader_scene_desc::USE_TXM_AS_DIFFUSE] = false;
        }

        if (material.is_state(res::Material::SPECULAR_TXM)) {
            desc.tex1 = rnd::get_system().get_texture_manager().require_texture(material.get_txm(res::Material::SPECULAR_TXM));
            desc.defines[rnd::shader_scene_desc::USE_SPECULAR_MAP] = true;
        }
        else {
            desc.defines[rnd::shader_scene_desc::USE_SPECULAR_MAP] = false;
        }

        if (material.is_state(res::Material::AMBIENT_TXM)) {
            desc.tex2 = rnd::get_system().get_texture_manager().require_texture(material.get_txm(res::Material::AMBIENT_TXM));
        }

        if (material.is_state(res::Material::NORMALS_TXM)) {
            desc.tex2 = rnd::get_system().get_texture_manager().require_texture(material.get_txm(res::Material::NORMALS_TXM));
            desc.defines[rnd::shader_scene_desc::USE_NORMAL_MAP] = true;
        }
        else {
            desc.defines[rnd::shader_scene_desc::USE_NORMAL_MAP] = false;
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

void scn::renderer_3d::draw_ecs_model(rnd::driver::driver_interface* drv, rnd::shader_scene_desc& scene)
{
    for (auto ent : ecs::filter<scn::model_root_component, scn::is_render_component_flag>()) {
        auto* transform = ecs::get_component<scn::transform_component>(ent);
        auto* root = ecs::get_component<scn::model_root_component>(ent);
        rnd::RENDER_MODE tmp = rnd::get_system().get_render_mode();

        if (auto* rnd_mode = ecs::get_component<rnd::render_mode_component>(ent)) {
            rnd::get_system().set_render_mode(rnd_mode->mode);
        }

        vertex_buffer->set_data(root->data.vertices);
        if (directional_light_count > 0) {
            scene.defines[rnd::shader_scene_desc::DIRECTION_LIGHT_COUNT] = true;
            scene.defines_values[rnd::shader_scene_desc::DIRECTION_LIGHT_COUNT] = std::to_string(directional_light_count);
        }

        if (point_light_count > 0) {
            scene.defines[rnd::shader_scene_desc::POINT_LIGHT_COUNT] = true;
            scene.defines_values[rnd::shader_scene_desc::POINT_LIGHT_COUNT] = std::to_string(point_light_count);
        }

        if (auto* anim = ecs::get_component<scn::playable_animation>(ent)) {
            scene.defines[rnd::shader_scene_desc::USE_ANIMATION] = true;
            scene.defines[rnd::shader_scene_desc::MAX_BONE_MATRICES_COUNT] = true;
            scene.defines_values[rnd::shader_scene_desc::MAX_BONE_MATRICES_COUNT] = "128";
            rnd::bones_matrices bones_matreces;
            bones_matreces.row_height = root->data.bones_data.original_size.x;
            bones_matreces.bone_count = root->data.bones_data.original_size.y;
            auto& bones = root->data.bones_matrices;
            if (bones.size() < rnd::bones_matrices::MAX_BONE_MATRICES_COUNT) {
                std::copy(bones.begin(), bones.end(), bones_matreces.bones);
                rnd::get_system().get_shader_manager().update_global_bones_matrices(bones_matreces, bones.size());
            } else {
                ASSERT_FAIL("Bones matrices count too big.");
            }
        }

        draw_ecs_meshes(ent, root->data, scene, drv);

        rnd::get_system().set_render_mode(tmp);
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

        if (auto* material = ecs::get_component<scn::material_link_component>(ent)) {
            apply_material(material->material, scene);
            //auto is_transparent = ecs::get_component<scn::is_transparent_flag_component>(material->material);
            //if (!is_transparent) {
                draw(scene, meshes->mesh, data, drv);
            //}
        }

    }

    if (auto* children = ecs::get_component<scn::children_component>(ent))
    {
        for (auto& child : children->children)
        {
            draw_ecs_meshes(child, data, scene, drv);
        }
    }
}

void scn::renderer_3d::draw_ecs_meshes_transparant(ecs::entity ent, const res::meshes_conteiner& data, rnd::shader_scene_desc& scene, rnd::driver::driver_interface* drv)
{
    if (auto* meshes = ecs::get_component<scn::mesh_component>(ent))
    {
        scene.uWorldMeshMatr = glm::mat4{ 1.0 };
        if (auto* transform = ecs::get_component<scn::transform_component>(ent))
        {
            scene.uWorldMeshMatr = transform->world;
        }

        if (auto* material = ecs::get_component<scn::material_link_component>(ent)) {
            apply_material(material->material, scene);
            auto is_transparent = ecs::get_component<scn::is_transparent_flag_component>(material->material);
            if (is_transparent) {
                draw(scene, meshes->mesh, data, drv);
            }        
        }

    }

    if (auto* children = ecs::get_component<scn::children_component>(ent))
    {
        for (auto& child : children->children)
        {
            draw_ecs_meshes_transparant(child, data, scene, drv);
        }
    }
}

void scn::renderer_3d::apply_material(ecs::entity material, rnd::shader_scene_desc& desc)
{
    if (auto* base_mlt = ecs::get_component<scn::base_material_component>(material))
    {
        desc.diffuseColor = base_mlt->albedo;
        desc.shininess = base_mlt->shininess;
        desc.emissiveColor = base_mlt->emissive;
        desc.defines[rnd::shader_scene_desc::USE_TXM_AS_DIFFUSE] = false;
        desc.defines[rnd::shader_scene_desc::USE_SPECULAR_MAP] = false;
        desc.defines[rnd::shader_scene_desc::USE_NORMAL_MAP] = false;
    }

    if (auto* albedo_txm = ecs::get_component<scn::albedo_map_component>(material))
    {
        desc.tex0 = rnd::get_system().get_texture_manager().require_texture(albedo_txm->txm);
        desc.defines[rnd::shader_scene_desc::USE_TXM_AS_DIFFUSE] = true;
    }

    if (auto* specular_txm = ecs::get_component<scn::specular_map_component>(material))
    {
        desc.tex1 = rnd::get_system().get_texture_manager().require_texture(specular_txm->txm);
        desc.defines[rnd::shader_scene_desc::USE_SPECULAR_MAP] = true;
    }

    if (auto* normal_txm = ecs::get_component<scn::normal_map_component>(material))
    {
        desc.tex2 = rnd::get_system().get_texture_manager().require_texture(normal_txm->txm);
        desc.defines[rnd::shader_scene_desc::USE_NORMAL_MAP] = true;
    }
}

void scn::renderer_3d::z_prepass(rnd::driver::driver_interface* drv)
{
    static res::Tag z_pass_tag = res::Tag(res::Tag::memory, "__z_prepass_rt");
    static res::Tag z_pass_color_tag = res::Tag(res::Tag::memory, "__z_prepass_color_rt");
    auto& txm_manager = rnd::get_system().get_texture_manager();
    rnd::global_params common_matrix;

    for (ecs::entity& ent : ecs::filter<scn::camera_component, scn::is_render_component_flag>())
    {
        scn::camera_component* camera = ecs::get_component<scn::camera_component>(ent);
        if (camera->viewport.size.x < 1 || camera->viewport.size.y < 1) {
            continue;
        }
        auto z_pass_rt = txm_manager.find(z_pass_tag);
        auto z_pass_color_rt = txm_manager.find(z_pass_color_tag);

        if (z_pass_rt && (z_pass_rt->size() != camera->viewport.size))
        {
            txm_manager.remove(z_pass_tag);
            txm_manager.remove(z_pass_color_tag);
            z_pass_rt = nullptr;
            z_pass_color_rt = nullptr;
        }

        if (!z_pass_rt) {
            rnd::driver::texture_header header;
            header.picture.height = camera->viewport.size.y;
            header.picture.width = camera->viewport.size.x;
            header.picture.channels = rnd::driver::texture_header::TYPE::D32;
            header.wrap = rnd::driver::texture_header::WRAPPING::CLAMP_TO_BORDER;
            header.mag = rnd::driver::texture_header::FILTERING::NEAREST;
            header.min = rnd::driver::texture_header::FILTERING::NEAREST;
            z_pass_rt = txm_manager.generate_texture(z_pass_tag, header);

            header.picture.channels = rnd::driver::texture_header::TYPE::RGBA8;
            z_pass_color_rt = txm_manager.generate_texture(z_pass_color_tag, header);
            egLOG("scn/renderer", "'{2}' Z-prepass size: {0}, {1}", z_pass_rt->width(), z_pass_rt->height(), z_pass_tag.name());
        }

        drv->disable(rnd::driver::ENABLE_FLAGS::COLOR_TEST);
        drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
        drv->enable(rnd::driver::ENABLE_FLAGS::DEPTH_MASK);

        drv->push_frame_buffer();
        drv->set_render_rarget(nullptr, z_pass_rt);
        drv->clear(rnd::driver::CLEAR_FLAGS::DEPTH_BUFFER);
        drv->clear(rnd::driver::CLEAR_FLAGS::COLOR_BUFFER);

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
        drv->set_viewport(camera->viewport);
        rnd::pass_z_prepass_desc z_pass;
        draw_ecs_model(drv, z_pass);

        drv->pop_frame_buffer();

        drv->enable(rnd::driver::ENABLE_FLAGS::COLOR_TEST);
        drv->disable(rnd::driver::ENABLE_FLAGS::DEPTH_TEST);
        drv->disable(rnd::driver::ENABLE_FLAGS::DEPTH_MASK);
    }
}

void scn::renderer_3d::draw_transparent(rnd::driver::driver_interface* drv)
{
    for (const auto& ent : ecs::filter<scn::model_root_component, scn::is_render_component_flag>())
    {
        auto* transform = ecs::get_component<scn::transform_component>(ent);
        auto* root = ecs::get_component<scn::model_root_component>(ent);
        rnd::RENDER_MODE tmp = rnd::get_system().get_render_mode();

        if (auto* rnd_mode = ecs::get_component<rnd::render_mode_component>(ent)) {
            rnd::get_system().set_render_mode(rnd_mode->mode);
        }

        vertex_buffer->set_data(root->data.vertices);
        rnd::pass_transparent_desc desc;
        if (directional_light_count > 0) {
            desc.defines[rnd::shader_scene_desc::DIRECTION_LIGHT_COUNT] = true;
            desc.defines_values[rnd::shader_scene_desc::DIRECTION_LIGHT_COUNT] = std::to_string(directional_light_count);
        }

        if (point_light_count > 0) {
            desc.defines[rnd::shader_scene_desc::POINT_LIGHT_COUNT] = true;
            desc.defines_values[rnd::shader_scene_desc::POINT_LIGHT_COUNT] = std::to_string(point_light_count);
        }

        if (auto* anim = ecs::get_component<scn::playable_animation>(ent)) {
            desc.defines[rnd::shader_scene_desc::USE_ANIMATION] = true;
            desc.defines[rnd::shader_scene_desc::MAX_BONE_MATRICES_COUNT] = true;
            desc.defines_values[rnd::shader_scene_desc::MAX_BONE_MATRICES_COUNT] = "128";
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
        }

        draw_ecs_meshes_transparant(ent, root->data, desc, drv);

        rnd::get_system().set_render_mode(tmp);
    }
}

void scn::renderer_3d::draw_composition(rnd::driver::driver_interface* drv, rnd::driver::texture_interface* color, rnd::driver::texture_interface* weight)
{
    res::Vertex screen[4];
    screen[0].position = glm::vec3{ -1, -1, 0 };
    screen[1].position = glm::vec3{ -1,  1, 0 };
    screen[2].position = glm::vec3{  1,  1, 0 };
    screen[3].position = glm::vec3{  1, -1, 0 };
    vertex_buffer->set_data_ptr(screen, 4);
    unsigned int indeces[] { 0, 1, 2, 0, 3, 2 };
    index_buffer->set_data_ptr(indeces, std::size(indeces));
    rnd::pass_composition_desc decs;
    decs.tex0 = color;
    decs.tex1 = weight;
    drv->set_viewport({ 0,0, color->width(), color->height() });
    rnd::configure_pass(decs);
    drv->draw_indeces(vertex_array, rnd::RENDER_MODE::TRIANGLE, std::size(indeces));
}

void scn::renderer_3d::draw_sky(rnd::driver::driver_interface* drv)
{ 
    for (const auto& sky : ecs::filter<scn::is_render_component_flag, scn::sky_component>()) {
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

    if (data.bones_data.bones_indeces_txm.is_valid()) {
        desc.tex3 = rnd::get_system().get_texture_manager().require_texture(data.bones_data.bones_indeces_txm);
    }

    rnd::configure_pass(desc);
    // draw mesh
    drv->draw_indeces(vertex_array, rnd::get_system().get_render_mode(), mesh.get_indices_count(), mesh.vx_begin);
}
