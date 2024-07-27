#pragma once
#include "../common/common.h"
#include "rnd_buffer_layout.h"

namespace render
{
	class VertexBuffer
	{
	public:
		VertexBuffer(std::size_t size);
		VertexBuffer(float* vertices, std::size_t size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		void set_data(const void* data, std::size_t size);

		template<class T>
		void set_data(const std::vector<T>& data) {
			set_data(data.data(), data.size() * sizeof(T));
		}

		const BufferLayout& get_layout() const  { return m_Layout; }
		void set_layout(const BufferLayout& layout)  { m_Layout = layout; }
	private:
		GLuint m_RendererID = 0;
		BufferLayout m_Layout;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* indices, std::size_t count);
		template<class T>
		IndexBuffer(const std::vector<T>& indices) 
			:IndexBuffer(indices.data(), indices.size()){}
		virtual ~IndexBuffer();

		void bind() const;
		void unbind() const;

		std::size_t get_count() const { return m_Count; }
	private:
		GLuint m_RendererID = 0;
		std::size_t m_Count = 0;
	};
}

namespace rnd = render;