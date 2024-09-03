#include "rnd_gl_cubemap.h" 
#include "open_gl_specific.h"

rnd::driver::gl::cubemap::~cubemap()
{
	glDeleteTextures(1, &id);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::cubemap::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	CHECK_GL_ERROR();
}
