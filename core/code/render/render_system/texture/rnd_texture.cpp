#include "rnd_texture.h"
#include <res_resource_system.h>
#include <res_resource_texture.h>

std::unique_ptr<render::driver::texture_interface> render::Texture::load(driver::driver_interface* drv, const res::Tag& tag)
{
	auto res = res::get_system().require_resource<res::Image>(tag);
    driver::texture_header header;
    header.data = res->data();
    header.channels = res->channels();
    header.width = res->size().x;
    header.height = res->size().y;
    header.wrap = driver::texture_header::WRAPPING::REPEAT;
    header.min = driver::texture_header::FILTERING::LINEAR_MIPMAP;
    header.mag = driver::texture_header::FILTERING::LINEAR;

	return drv->create_texture(header);
}