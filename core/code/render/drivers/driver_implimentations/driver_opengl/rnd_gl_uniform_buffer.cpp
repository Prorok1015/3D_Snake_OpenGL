#include "rnd_gl_uniform_buffer.h"

#define CHECK_GL_ERROR()

render::driver::gl::uniform_buffer::uniform_buffer(std::size_t size, std::size_t binding)
{
	glGenBuffers(1, &m_RendererID);

	CHECK_GL_ERROR();
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	CHECK_GL_ERROR();
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	CHECK_GL_ERROR();

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_RendererID, 0, size);
	CHECK_GL_ERROR();
}

render::driver::gl::uniform_buffer::~uniform_buffer()
{
	glDeleteBuffers(1, &m_RendererID);
	CHECK_GL_ERROR();
}

void render::driver::gl::uniform_buffer::set_data(const void* data, std::size_t size, std::size_t offset)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	CHECK_GL_ERROR();
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	CHECK_GL_ERROR();
}
