#pragma once
#include "../common/common.h"
#include "../render/texture/rnd_texture.h"
#include "../render/texture/rnd_texture_manager.h"
#include "../resource/res_resource_model.h"
#include "mesh.h"

namespace scene {

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