#include "rnd_texture_manager.h"

std::shared_ptr<rnd::Texture> rnd::TextureManager::require_texture(const res::Tag& tag) const
{
	auto it = cache.find(tag);
	if (it != cache.end()) {
		auto& texture = it->second;
		return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
	}

	auto& texture = cache[tag] = rnd::Texture::load(drv, tag);
	return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
}

std::shared_ptr<rnd::Texture> rnd::TextureManager::generate_texture(const res::Tag& tag, glm::ivec2 size, int channels, std::vector<unsigned char> data) const
{
	auto it = cache.find(tag);
	if (it != cache.end()) {
		auto& texture = it->second;
		return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
	}

    driver::texture_header header;
    header.data = data.data();
    header.channels = channels;
    header.width = size.x;
    header.height = size.y;
    header.wrap = driver::texture_header::WRAPPING::REPEAT;
    header.min = driver::texture_header::FILTERING::LINEAR_MIPMAP;
    header.mag = driver::texture_header::FILTERING::LINEAR;
	auto& texture = cache[tag] = drv->create_texture(header);
    return std::make_shared<Texture>(texture.get(), header.width, header.height);
}
 
