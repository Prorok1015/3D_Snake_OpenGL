#include "rnd_gl_buffer.h"
#include "open_gl_specific.h"

const GLint gBufferTypeToGlBufferType[] =
{
	GL_ARRAY_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER,
};

render::driver::gl::buffer::buffer()
{
	glCreateBuffers(1, &buffer_id);
	CHECK_GL_ERROR();
}

render::driver::gl::buffer::~buffer()
{
	glDeleteBuffers(1, &buffer_id);
	CHECK_GL_ERROR();
}

void render::driver::gl::buffer::bind()
{
	glBindBuffer(buffer_type, buffer_id);
	CHECK_GL_ERROR();
}

void render::driver::gl::buffer::unbind()
{
	glBindBuffer(buffer_type, 0);
	CHECK_GL_ERROR();
}

void render::driver::gl::buffer::set_data(const void* data, std::size_t size, BUFFER_BINDING binding, BUFFER_TYPE type)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	CHECK_GL_ERROR();

	buffer_type = gBufferTypeToGlBufferType[(int)type];

	if (data && isAllocatedMemory) {
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		CHECK_GL_ERROR();
	} else {
		isAllocatedMemory = true;
		glBufferData(GL_ARRAY_BUFFER, size, data, (binding == BUFFER_BINDING::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
		CHECK_GL_ERROR();
	}
}
