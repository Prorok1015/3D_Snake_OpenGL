#pragma once
#include <common.h>
#include <texture/rnd_texture.h>
#include <texture/rnd_texture_manager.h>
#include <res_resource_model.h>
#include "scn_mesh.h"
#include "ecs/ecs_entity.h"

namespace scn {

    struct model_comonent {
        std::vector<res::Mesh> meshes;
        std::shared_ptr<res::Model> model;
    };

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

    class Model
    {
    public:
        std::vector<res::Mesh> meshes;
        glm::mat4 model{ 1.0f };

        std::shared_ptr<res::Model> res_ = nullptr;

        Model() = default;
        // constructor, expects a filepath to a 3D model.
        Model(std::string_view path);
        Model(std::shared_ptr<res::Model> res_model);

    private:
        void loadModel();
    };

}