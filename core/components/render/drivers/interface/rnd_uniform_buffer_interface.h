#pragma once

namespace render::driver
{
	class uniform_buffer_interface
	{
	public:
		virtual ~uniform_buffer_interface() {}
		virtual void set_data(const void* data, std::size_t size, std::size_t offset = 0) = 0;
	};
}