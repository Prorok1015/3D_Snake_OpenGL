#include "model.h"
#include <filesystem>
#include "../resource/res_resource_system.h"

using namespace scene;


scene::Model::Model(std::string_view path, bool gamma)
    : gammaCorrection(gamma)
{
    loadModel(path);
}

void scene::Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

std::vector<std::shared_ptr<rnd::Texture>> make_texture(std::vector<res::Material> vec)
{
    std::vector<std::shared_ptr<rnd::Texture>> result;
    for (const auto& mat : vec) {
        auto txt = rnd::TextureManager::inst().require_texture(mat.tag_);
        txt->tmp_type = mat.type_;
        result.push_back(txt);
    }

    return result;
}

void scene::Model::loadModel(std::string_view path)
{
    res_ = res::get_system().require_resource<res::Model>(res::Tag::make(path));
    meshes.clear();
    for (const auto& mesh : res_->get_meshes()) {
        meshes.push_back(Mesh{ mesh.vertexes_, mesh.indeces_, make_texture(mesh.textures_) });
    }
}

