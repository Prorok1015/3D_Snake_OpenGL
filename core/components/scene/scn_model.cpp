#include "scn_model.h"
#include <filesystem>
#include <res_resource_system.h>
#include <rnd_render_system.h>

using namespace scene;


scene::Model::Model(std::string_view path, bool gamma)
{
    loadModel(path);
}

std::vector<std::shared_ptr<rnd::Texture>> make_texture(std::vector<res::Material> vec)
{
    std::vector<std::shared_ptr<rnd::Texture>> result;
    for (const auto& mat : vec) {
        auto txt = rnd::get_system().get_texture_manager().require_texture(mat.tag_);
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
        meshes.push_back(Mesh{ mesh.vertexes_, mesh.indeces_, mesh.textures_.empty() ? res::Tag::make("__black") : mesh.textures_.front().tag_});
    }
}

