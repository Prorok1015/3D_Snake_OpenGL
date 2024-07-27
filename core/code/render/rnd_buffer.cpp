#include "rnd_buffer.h"
#include "rnd_render_system.h"

render::VertexBuffer::VertexBuffer(std::size_t size)
{
	glCreateBuffers(1, &m_RendererID);
	//m_RendererID = rnd::get_system().gen_buf();
	CHECK_GL_ERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();

	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	CHECK_GL_ERROR();
}

render::VertexBuffer::VertexBuffer(float* vertices, std::size_t size)
{
	glCreateBuffers(1, &m_RendererID);
	//m_RendererID = rnd::get_system().gen_buf();
	CHECK_GL_ERROR();
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();
}

render::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
	CHECK_GL_ERROR();
}

void render::VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
}

void render::VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERROR();
}

void render::VertexBuffer::set_data(const void* data, std::size_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	CHECK_GL_ERROR();
}

/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

render::IndexBuffer::IndexBuffer(const unsigned int* indices, std::size_t count)
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

render::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
	CHECK_GL_ERROR();
}

void render::IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	CHECK_GL_ERROR();
}

void render::IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CHECK_GL_ERROR();
}