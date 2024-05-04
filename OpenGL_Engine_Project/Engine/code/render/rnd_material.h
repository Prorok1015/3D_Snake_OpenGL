#pragma once
#include "shader/rnd_shader_manager.h"
#include "texture/rnd_texture_manager.h"
#include "../resource/res_resource_tag.h"

namespace render
{
	class Material
	{
	public:
		res::Tag shader = res::Tag::make("scene");
		res::Tag texture = res::Tag::make("block.png");
		bool is_self_indecex = false;
	};
}

namespace rnd = render;