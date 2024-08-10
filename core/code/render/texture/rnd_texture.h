#pragma once
#include "../../common/common.h"
#include "../../resource/res_resource_tag.h"
#include <rnd_texture_interface.h>
#include <rnd_driver_interface.h>

namespace render
{
	class Texture
	{
	public:
		static std::unique_ptr<driver::texture_interface> load(driver::driver_interface* drv, const res::Tag& tag);

	public:
		Texture(driver::texture_interface* texture, int width, int height) 
			: texture_impl(texture), width_(width), height_(height) {
		}
		~Texture() {}

		void bind() {
			texture_impl->bind();
		}

		int width() const { return width_; }
		int height() const { return height_; }

		std::string tmp_type;

	protected:
		driver::texture_interface* texture_impl = nullptr;
		int width_;
		int height_;
	};
}

namespace rnd = render;