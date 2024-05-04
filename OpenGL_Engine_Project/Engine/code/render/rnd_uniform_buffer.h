#pragma once
#include "../common/common.h"

namespace render
{
	class UniformBuffer
	{
	public:
		UniformBuffer(std::size_t size, std::size_t binding);
		~UniformBuffer();

		void set_data(const void* data, std::size_t size, std::size_t offset = 0);
		
		void set_data(auto&& data, std::size_t offset = 0) {
			set_data(glm::value_ptr(data), sizeof(decltype(data)), offset);
		}
	private:
		GLuint m_RendererID = 0;
	};
}

namespace rnd = render;
