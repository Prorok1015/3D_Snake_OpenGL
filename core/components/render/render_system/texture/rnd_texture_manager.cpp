#include "rnd_texture_manager.h"
#include "res_system.h"
#include "res_picture.h"

std::shared_ptr<rnd::Texture> rnd::TextureManager::require_texture(const res::Tag& tag)
{
	auto it = cache.find(tag);
	if (it != cache.end()) {
		auto& texture = it->second;
		return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
	}

	auto& texture = cache[tag] = rnd::Texture::load(drv, tag);
	return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
}

std::shared_ptr<rnd::Texture> rnd::TextureManager::require_cubemap_texture(const std::vector<res::Tag>& tags)
{
	auto it = cache.find(tags.front());
	if (it != cache.end()) {
		auto& texture = it->second;
		return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
	}

	rnd::driver::cubmap_texture_header cb_header;

	auto add = [](rnd::driver::texture_header::data& header, res::Tag tag)
	{
		auto res = res::get_system().require_resource<res::Picture>(tag);

		header.data = res->data();
		switch (res->channels())
		{
		case 1: header.channels = driver::texture_header::TYPE::R8; break;
		case 3: header.channels = driver::texture_header::TYPE::RGB8; break;
		case 4: header.channels = driver::texture_header::TYPE::RGBA8; break;
		default:
			break;
		}
		header.width = res->size().x;
		header.height = res->size().y;
	};
	 
	add(cb_header.right, tags[0]);
	add(cb_header.left, tags[1]);
	add(cb_header.top, tags[2]);
	add(cb_header.bottom, tags[3]);
	add(cb_header.front, tags[4]);
	add(cb_header.back, tags[5]);
	cb_header.wrap = rnd::driver::texture_header::WRAPPING::CLAMP_TO_EDGE;
	cb_header.min = rnd::driver::texture_header::FILTERING::LINEAR;
	cb_header.mag = rnd::driver::texture_header::FILTERING::LINEAR;

	auto& texture = cache[tags.front()] = drv->create_texture(cb_header);
	return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
}

std::shared_ptr<rnd::Texture> rnd::TextureManager::generate_texture(const res::Tag& tag, glm::ivec2 size, rnd::driver::texture_header::TYPE channels, std::vector<unsigned char> data)
{
	auto it = cache.find(tag);
	if (it != cache.end()) {
		auto& texture = it->second;
		return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
	}

    driver::texture_header header;
    header.picture.data = data.data();
    header.picture.channels = channels;
    header.picture.width = size.x;
    header.picture.height = size.y;
    header.wrap = driver::texture_header::WRAPPING::REPEAT;
    header.min = driver::texture_header::FILTERING::LINEAR_MIPMAP;
    header.mag = driver::texture_header::FILTERING::LINEAR;
	auto& texture = cache[tag] = drv->create_texture(header);
    return std::make_shared<Texture>(texture.get(), header.picture.width, header.picture.height);
}

std::shared_ptr<rnd::Texture> rnd::TextureManager::generate_texture(const res::Tag& tag, rnd::driver::texture_header header)
{
	auto it = cache.find(tag);
	if (it != cache.end()) {
		auto& texture = it->second;
		return std::make_shared<Texture>(texture.get(), texture->width(), texture->height());
	}

	auto& texture = cache[tag] = drv->create_texture(header);
    return std::make_shared<Texture>(texture.get(), header.picture.width, header.picture.height);
}
 

void rnd::TextureManager::clear_cache()
{ 
	std::vector<res::Tag> list_to_delete;
	for (auto& [key, _] : cache)
	{
		if (key.protocol() != "memory") {
			list_to_delete.push_back(key);
		}
	}

	for (auto& key : list_to_delete) {
		cache.erase(key);
	}
}