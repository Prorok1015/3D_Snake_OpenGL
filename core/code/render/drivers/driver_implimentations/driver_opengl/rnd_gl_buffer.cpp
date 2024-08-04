#include "rnd_gl_buffer.h"

//#include <common/open_gl_specific.h>
#define CHECK_GL_ERROR()

render::driver::gl::vertex_buffer::vertex_buffer(std::size_t size)
{
	glCreateBuffers(1, &m_RendererID);
	//m_RendererID = rnd::get_system().gen_buf();
	CHECK_GL_ERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();

	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	CHECK_GL_ERROR();
}

render::driver::gl::vertex_buffer::vertex_buffer(float* vertices, std::size_t size)
{
	glCreateBuffers(1, &m_RendererID);
	//m_RendererID = rnd::get_system().gen_buf();
	CHECK_GL_ERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();
}

render::driver::gl::vertex_buffer::~vertex_buffer()
{
	glDeleteBuffers(1, &m_RendererID);
	CHECK_GL_ERROR();
}

void render::driver::gl::vertex_buffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
}

void render::driver::gl::vertex_buffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERROR();
}

void render::driver::gl::vertex_buffer::set_data(const void* data, std::size_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	CHECK_GL_ERROR();
}

/////////////////////////////////////////////////////////////////////////////
// index_buffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

render::driver::gl::index_buffer::index_buffer(const unsigned int* indices, std::size_t count)
	: m_Count(count)
{

	glCreateBuffers(1, &m_RendererID);
	CHECK_GL_ERROR();
	//m_RendererID = rnd::get_system().gen_buf();
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();
}

render::driver::gl::index_buffer::~index_buffer()
{
	glDeleteBuffers(1, &m_RendererID);
	CHECK_GL_ERROR();
}

void render::driver::gl::index_buffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
}

void render::driver::gl::index_buffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CHECK_GL_ERROR();
}