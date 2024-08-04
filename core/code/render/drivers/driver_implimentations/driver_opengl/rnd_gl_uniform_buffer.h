#pragma once
#include <glad/glad.h> 
#include <vector>

namespace glm
{
	decltype(auto) value_ptr(auto&& v)
	{
		return std::addressof(v);
	}
}

namespace render::driver::gl
{
	class uniform_buffer
	{
	public:
		uniform_buffer(std::size_t size, std::size_t binding);
		~uniform_buffer();

		void set_data(const void* data, std::size_t size, std::size_t offset = 0);

		void set_data(auto&& data, std::size_t offset = 0) {
			set_data(glm::value_ptr(data), sizeof(decltype(data)), offset);
		}
	private:
		GLuint m_RendererID = 0;
	};
}