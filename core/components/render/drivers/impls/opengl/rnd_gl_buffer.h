#pragma once
#include <vector>
#include <glad/glad.h>
#include <rnd_buffer_interface.h>
#include <rnd_buffer_layout.h>

namespace render::driver::gl
{
	class buffer : public render::driver::buffer_interface
	{
	public:
		buffer();
		virtual ~buffer() override;

		void bind();
		void unbind();
		virtual void set_data(const void* data, std::size_t size, BUFFER_BINDING binding, BUFFER_TYPE type) override;

		virtual const BufferLayout& get_layout() const override { return layout; }
		virtual void set_layout(const BufferLayout& layout_) override { layout = layout_; }

	private:
		bool isAllocatedMemory = false;
		GLuint buffer_id = 0;
		GLint buffer_type;
		BufferLayout layout;
	};
}