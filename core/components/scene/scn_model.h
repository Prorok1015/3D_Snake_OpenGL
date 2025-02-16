#pragma once
#include <common.h>
#include <texture/rnd_texture.h>
#include <texture/rnd_texture_manager.h>
#include <res_resource_model.h>
#include "ecs_entity.h"
#include "scn_primitives.h"
#include <boost/json.hpp>

namespace json = boost::json;

namespace scn {

    struct keyframes_component {
        std::unordered_map<std::string, res::animation_node> keyframes;
    };

    struct name_component {
        std::string name;
    };

    struct mesh_component {
        res::mesh_view mesh;
    };

    struct model_root_component {
        res::meshes_conteiner data;
    };

    struct animations_component {
        std::vector<res::animation> animations;
    };

    struct bone_component {
        glm::mat4 offset{ 1.0 };
    };

    struct parent_component {
        ecs::entity parent;
    };

    struct children_component {
        std::vector<ecs::entity> children;
    };
    /*
    struct transform_component {
        glm::mat4 world = glm::mat4{ 1.0 };
        glm::mat4 local = glm::mat4{ 1.0 };
    };*/

    struct local_transform {
        glm::mat4 local = glm::mat4{ 1.0 };
    };

    struct world_transform {
        glm::mat4 world = glm::mat4{ 1.0 };
    };

    struct scene_anchor_component {

    };

    struct is_render_component_flag {

    };

    struct sky_component
    {
        res::meshes_conteiner data;
        res::mesh_view mesh;
        std::vector<res::Tag> cube_map;
    };

    struct directional_light
    {
        glm::vec4 direction;
        glm::vec4 diffuse;
        glm::vec4 ambient;
        glm::vec4 specular;
    };

    struct playable_animation
    {
        std::string name;
        float current_tick = 0.f;
        bool is_repeat_animation = true;
    };

    // Конвертация в JSON
    void tag_invoke(json::value_from_tag, json::value& jv, const keyframes_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const name_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const mesh_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const model_root_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const animations_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const bone_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const parent_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const children_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const local_transform& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const scene_anchor_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const is_render_component_flag& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const sky_component& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const directional_light& c);
    void tag_invoke(json::value_from_tag, json::value& jv, const playable_animation& c);

    json::value to_json(const ecs::entity& e);
}