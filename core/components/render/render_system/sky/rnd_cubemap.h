#pragma once
#include <res_resource_tag.h>
#include <scn_mesh.h>
namespace rnd
{
	struct cubemap_component
	{
		scn::Mesh mesh;
		std::vector<res::Tag> cube_map;
	};
}