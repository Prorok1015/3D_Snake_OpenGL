#pragma once

namespace render::driver
{
	class buffer_interface
	{
	public:
		virtual ~buffer_interface() {}
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void set_data(const void* data, size_t size) = 0;
	};
}