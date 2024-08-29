#pragma once
#include "shader/rnd_shader_manager.h"
#include "texture/rnd_texture_manager.h"
#include <res_resource_tag.h>

namespace rnd
{
	class Material
	{
	public:
		res::Tag shader_tag = res::Tag::make("scene");
		res::Tag texture_tag = res::Tag::make("block.png");
		bool is_self_indecex = false;
	};
}
