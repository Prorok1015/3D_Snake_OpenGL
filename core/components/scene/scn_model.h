#pragma once
#include <common.h>
#include <texture/rnd_texture.h>
#include <texture/rnd_texture_manager.h>
#include <res_resource_model.h>
#include "ecs/ecs_entity.h"
#include "scn_primitives.h"

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

    struct transform_component {
        glm::mat4 world = glm::mat4{ 1.0 };
        glm::mat4 local = glm::mat4{ 1.0 };
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

    struct light_point
    {
        glm::vec4 position;
        glm::vec4 diffuse;
        glm::vec4 ambient;
        glm::vec4 specular;
    };
}