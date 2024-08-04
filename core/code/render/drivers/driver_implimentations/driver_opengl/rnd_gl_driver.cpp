#include "rnd_gl_driver.h"
#include <glad/glad.h>

#define CHECK_GL_ERROR()

void render::driver::gl::driver::set_viewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void render::driver::gl::driver::set_clear_color(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void render::driver::gl::driver::clear(int flags)
{
	glClear(flags);
}

void render::driver::gl::driver::set_activate_texture(int idx)
{
	// active proper texture unit before binding
	glActiveTexture(GL_TEXTURE0 + idx);
	CHECK_GL_ERROR();
}

void render::driver::gl::driver::set_line_size(float size)
{
	glLineWidth(size);
	CHECK_GL_ERROR();
}

void render::driver::gl::driver::set_point_size(float size)
{
	glPointSize(size);
	CHECK_GL_ERROR();
}

void render::driver::gl::driver::draw_elements(unsigned int render_mode, unsigned int vao, unsigned int count)
{
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	glDrawElements((GLenum)render_mode, count, GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void render::driver::gl::driver::draw_elements(unsigned int render_mode, unsigned int count)
{
	glDrawElements((GLenum)render_mode, count, GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
}

void render::driver::gl::driver::enable(int flags)
{
	glEnable(flags);
}
