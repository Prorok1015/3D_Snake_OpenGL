#include "rnd_gl_texture.h"
#include "open_gl_specific.h"

rnd::driver::gl::texture::texture(GLuint texture_id, const texture_header& hdr)
	: id(texture_id)
	, texture_interface(hdr)
{
	width_i = hdr.picture.width;
	height_i = hdr.picture.height;
}

rnd::driver::gl::texture::~texture()
{
	glDeleteTextures(1, &id);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::texture::bind(unsigned int idx)
{
	glBindTextureUnit(idx, id);
	CHECK_GL_ERROR();
}

int rnd::driver::gl::texture::width() const
{
	return width_i;
}

int rnd::driver::gl::texture::height() const
{
	return height_i;
}

glm::ivec2 rnd::driver::gl::texture::size() const
{
	return { width_i , height_i };
}
