#include "rnd_gl_cubemap.h" 
#include "open_gl_specific.h"

rnd::driver::gl::cubemap::~cubemap()
{
	glDeleteTextures(1, &id);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::cubemap::bind(unsigned int idx)
{
	glBindTextureUnit(idx, id);
	CHECK_GL_ERROR();
}
