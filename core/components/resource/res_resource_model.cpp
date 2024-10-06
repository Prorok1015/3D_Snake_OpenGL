#include "common.h"
#include "res_resource_model.h"
#include "res_resource_texture.h"
#include "res_resource_system.h"
#include "res_model_loader.h"


res::Model::Model(const Tag& tag)
	: Resource(tag)
{
	loader::model_loader ld(tag);
    meshes = ld.load();
	model_hierarchy = ld.model_hierarchy;
	model = ld.model;
}