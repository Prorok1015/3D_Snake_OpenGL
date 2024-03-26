#pragma once
#include "../../common/common.h"
#include "../../resource/res_resource_tag.h"

namespace render
{
	class Texture;

	class TextureManager
	{
	public:
		std::shared_ptr<Texture> require_texture(const res::Tag& tag) const;

		static TextureManager& inst() {
			static TextureManager inst;
			return inst;
		}

	protected:
		std::shared_ptr<Texture> find_cache(const res::Tag& tag) const;

	protected:
		mutable std::unordered_map<res::Tag, std::shared_ptr<Texture>, res::Tag::hash> _cache;
	};

}

namespace rnd = render;