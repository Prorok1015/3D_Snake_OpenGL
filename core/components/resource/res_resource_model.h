#pragma once
#include <common.h>
#include "res_resource_base.h"
#include "res_mesh.hpp"

namespace res
{  
	class Model : public Resource
	{
	public:
		Model() = default;
		Model(const Tag& tag);

		const std::vector<Mesh>& get_meshes() const { return meshes; }
		res::node_hierarchy& get_hierarchy() { return model_hierarchy; }
		res::model_presintation& get_model_pres() { return model; }

	private:
		std::vector<Mesh> meshes;
		res::node_hierarchy model_hierarchy;
		res::model_presintation model;
	};
}
