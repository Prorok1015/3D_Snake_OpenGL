#pragma once
#include <common.h>
#include "res_tag.h"
#include "rnd_texture.h"

#include <rnd_driver_interface.h>
#include <rnd_texture_interface.h>

namespace rnd
{
	class TextureManager
	{
	public:
		TextureManager(driver::driver_interface* driver)
			: drv(driver) {}

		std::shared_ptr<Texture> require_texture(const res::Tag& tag);
		std::shared_ptr<Texture> require_cubemap_texture(const std::vector<res::Tag>& tags);
		std::shared_ptr<Texture> generate_texture(const res::Tag& tag, glm::ivec2 size, rnd::driver::texture_header::TYPE channels, std::vector<unsigned char> data);
		std::shared_ptr<Texture> generate_texture(const res::Tag& tag, rnd::driver::texture_header header);

		void clear_cache();
	protected:
		mutable std::unordered_map<res::Tag, std::unique_ptr<driver::texture_interface>, res::Tag::hash> cache;
		driver::driver_interface* drv = nullptr;
	};

}