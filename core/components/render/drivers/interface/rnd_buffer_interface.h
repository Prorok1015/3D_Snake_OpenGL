#pragma once
#include "rnd_buffer_layout.h"

namespace render::driver
{
	enum class BUFFER_BINDING
	{
		STATIC, DYNAMIC
	};

	enum class BUFFER_TYPE
	{
		ARRAY_BUFFER,
		ELEMENT_ARRAY_BUFFER
	};

	class buffer_interface
	{
	public:
		virtual ~buffer_interface() {}
		virtual void set_data(const void* data, size_t size, BUFFER_BINDING binding, BUFFER_TYPE type = BUFFER_TYPE::ARRAY_BUFFER) = 0;

		virtual const BufferLayout& get_layout() const = 0;
		virtual void set_layout(const BufferLayout& layout_) = 0;
	};
}