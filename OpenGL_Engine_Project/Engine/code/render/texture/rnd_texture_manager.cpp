#include "rnd_texture_manager.h"
#include "../../resource/res_resource_system.h"
#include "../../resource/res_resource_texture.h"
#include "rnd_texture.h"

std::shared_ptr<render::Texture> render::TextureManager::require_texture(const res::Tag& tag) const
{
	if (auto ptr = find_cache(tag)) {
		return ptr;
	}

	return _cache[tag] = render::Texture::load(tag);
}

std::shared_ptr<render::Texture> render::TextureManager::find_cache(const res::Tag& tag) const
{
	auto it = _cache.find(tag);
	if (it != _cache.end()) {
		return it->second;
	}

	return nullptr;
}
