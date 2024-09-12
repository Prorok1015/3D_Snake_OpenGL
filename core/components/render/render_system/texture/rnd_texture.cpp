#include "rnd_texture.h"
#include <res_resource_system.h>
#include <res_resource_texture.h>

std::unique_ptr<rnd::driver::texture_interface> rnd::Texture::load(driver::driver_interface* drv, const res::Tag& tag)
{
	auto res = res::get_system().require_resource<res::Picture>(tag, true);
    driver::texture_header header;
    header.picture.data = res->data();
    header.picture.channels = res->channels();
    header.picture.width = res->size().x;
    header.picture.height = res->size().y;
    header.wrap = driver::texture_header::WRAPPING::REPEAT;
    header.min = driver::texture_header::FILTERING::LINEAR_MIPMAP;
    header.mag = driver::texture_header::FILTERING::LINEAR;

	return drv->create_texture(header);
}