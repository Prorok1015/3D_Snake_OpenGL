#pragma once
#include "../common/common.h"
#include "rnd_buffer.h"

namespace render
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void bind() const;
		void unbind() const;

		void add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void set_index_buffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& get_vertex_buffers() const { return m_VertexBuffers; }
		const std::shared_ptr<IndexBuffer>& get_index_buffer() const { return m_IndexBuffer; }
		uint32_t m_RendererID;
	private:
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}

namespace rnd = render;