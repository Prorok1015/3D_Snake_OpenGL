#pragma once
#include <common.h>
#include <res_resource_tag.h>
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

		std::shared_ptr<Texture> require_texture(const res::Tag& tag) const;
		std::shared_ptr<Texture> generate_texture(const res::Tag& tag, glm::ivec2 size, int channels, std::vector<unsigned char> data) const;

	protected:
		mutable std::unordered_map<res::Tag, std::unique_ptr<driver::texture_interface>, res::Tag::hash> cache;
		driver::driver_interface* drv = nullptr;
	};

}