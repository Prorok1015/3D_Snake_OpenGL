#pragma once
#include <common.h>
#include "scn_model.h"

namespace scn
{
	struct model_web
	{
		res::meshes_conteiner data;
		res::mesh_view mesh;
	};

	struct model_cube
	{
		res::meshes_conteiner data;
		res::mesh_view mesh;
	};

	struct model_sphere
	{
		res::meshes_conteiner data;
		res::mesh_view mesh;
	};

	model_web generate_web(glm::ivec2 size);

	model_cube generate_cube();

	model_sphere generate_sphere();
}