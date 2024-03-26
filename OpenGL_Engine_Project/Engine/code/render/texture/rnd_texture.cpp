#include "rnd_texture.h"
#include "../../resource/res_resource_system.h"
#include "../../resource/res_resource_texture.h"

std::shared_ptr<render::Texture> render::Texture::load(const res::Tag& tag)
{
	auto res = res::get_system().require_resource<res::Texture>(tag);
    FILTERING minfiltering = Texture::FILTERING::LINEAR_MIPMAP;
    FILTERING magfiltering = Texture::FILTERING::LINEAR;
    WRAPPING wrapping = Texture::WRAPPING::REPEAT;

    GLsizei t_width = res->size().x;
    GLsizei t_height = res->size().y;
    GLenum format = 0;
    if (res->channels() == 1)
        format = GL_RED;
    else if (res->channels() == 3)
        format = GL_RGB;
    else if (res->channels() == 4)
        format = GL_RGBA;
    GLubyte* image_data = res->data();
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

	return std::make_shared<Texture>(texture, t_width, t_height);
}