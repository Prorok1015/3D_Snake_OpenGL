#pragma once
#include "../texture/rnd_texture.h"
namespace rnd
{
	struct cubemap_component
	{
		std::shared_ptr<rnd::Texture> cube_map;
	};
}