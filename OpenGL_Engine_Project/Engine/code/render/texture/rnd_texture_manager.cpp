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

std::shared_ptr<render::Texture> render::TextureManager::generate_texture(const res::Tag& tag, glm::vec2 size, int channels, std::vector<unsigned char> data) const
{
	if (auto ptr = find_cache(tag)) {
		return ptr;
	}

    Texture::FILTERING minfiltering = Texture::FILTERING::LINEAR_MIPMAP;
    Texture::FILTERING magfiltering = Texture::FILTERING::LINEAR;
    Texture::WRAPPING wrapping = Texture::WRAPPING::REPEAT;

    GLsizei t_width = size.x;
    GLsizei t_height = size.y;
    GLenum format = 0;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;
    GLubyte* image_data = data.data();
    GLuint texture;
    glGenTextures(1, &texture);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture);
    CHECK_GL_ERROR();
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minfiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magfiltering);
    CHECK_GL_ERROR();
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapping);
    CHECK_GL_ERROR();

    glTexImage2D(GL_TEXTURE_2D, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    CHECK_GL_ERROR();
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();

    return _cache[tag] = std::make_shared<Texture>(texture, t_width, t_height);

}

std::shared_ptr<render::Texture> render::TextureManager::find_cache(const res::Tag& tag) const
{
	auto it = _cache.find(tag);
	if (it != _cache.end()) {
		return it->second;
	}

	return nullptr;
}
