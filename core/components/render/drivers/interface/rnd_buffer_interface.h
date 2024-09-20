#pragma once
#include "rnd_buffer_layout.h"

namespace rnd::driver
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
		virtual void set_data(const void* data, size_t size, BUFFER_BINDING binding) = 0;

		template<class T>
		void set_data(const std::vector<T>& data, BUFFER_BINDING binding = BUFFER_BINDING::DYNAMIC) {
			set_data(data.data(), data.size() * sizeof(T), binding);
		}

		virtual const BufferLayout& get_layout() const = 0;
		virtual void set_layout(const BufferLayout& layout_) = 0;
	};
}