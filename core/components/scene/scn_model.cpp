#include "scn_model.h"
#include <filesystem>
#include <res_resource_system.h>
#include <rnd_render_system.h>


scn::Model::Model(std::string_view path)
{
    res_ = res::get_system().require_resource<res::Model>(res::Tag::make(path));
    loadModel();
}

scn::Model::Model(std::shared_ptr<res::Model> res_model)
{
    res_ = res_model;
    loadModel();
}

void scn::Model::loadModel()
{
    meshes = res_->get_meshes();
}

