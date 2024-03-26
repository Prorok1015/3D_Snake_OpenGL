#pragma once
#include "../common/common.h"
#include "../render/texture/rnd_texture.h"
#include "../render/texture/rnd_texture_manager.h"
#include "../resource/res_resource_model.h"
#include "mesh.h"

namespace scene {
    rnd::Texture TextureFromFile(const std::string_view path, const std::string& directory, bool gamma = false);

    class Model
    {
    public:
        std::vector<Mesh> meshes;
        bool gammaCorrection;

        std::shared_ptr<res::Model> res_;

        // constructor, expects a filepath to a 3D model.
        Model(std::string_view path, bool gamma = false);

        // draws the model, and thus all its meshes
        void Draw(Shader& shader);

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string_view path);
    };

}