#pragma once
#include <common.h>
#include <texture/rnd_texture.h>
#include <texture/rnd_texture_manager.h>
#include <res_resource_model.h>
#include "scn_mesh.h"

namespace scn {

    struct model_comonent {
        std::vector<Mesh> meshes;
    };

    struct is_render_component_flag {

    };

    class Model
    {
    public:
        std::vector<Mesh> meshes;
        glm::mat4 model{ 1.0f };

        std::shared_ptr<res::Model> res_ = nullptr;

        Model() = default;
        // constructor, expects a filepath to a 3D model.
        Model(std::string_view path, bool gamma = false);

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string_view path);
    };

}