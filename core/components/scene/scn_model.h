#pragma once
#include <common.h>
#include <texture/rnd_texture.h>
#include <texture/rnd_texture_manager.h>
#include <res_resource_model.h>
#include "scn_mesh.h"

namespace scn {

    struct model_comonent {
        std::vector<res::Mesh> meshes;
    };

    struct transform_component {
        glm::mat4 world;
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