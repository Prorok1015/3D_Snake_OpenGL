#pragma once
#include <res_resource_tag.h>
#include <res_resource_model.h>

namespace rnd
{
	struct cubemap_component
	{
		res::Mesh mesh;
		std::vector<res::Tag> cube_map;
	};
}