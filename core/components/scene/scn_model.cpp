#include "scn_model.h"
#include <filesystem>
#include <res_resource_system.h>
#include <rnd_render_system.h>


scn::Model::Model(std::string_view path)
{
    loadModel(path);
}

void scn::Model::loadModel(std::string_view path)
{
    res_ = res::get_system().require_resource<res::Model>(res::Tag::make(path));
    meshes.clear();
    for (const auto& mesh : res_->get_meshes()) {
        meshes.push_back(Mesh{ mesh.vertexes, mesh.indeces, mesh.material });
    }
}

