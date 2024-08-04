#pragma once
#include <rnd_vertex_array_interface.h>
#include <rnd_gl_buffer.h>
#include <vector>
#include <memory>

namespace render::driver::gl
{
	class vertex_array : public render::driver::vertex_array_interface
	{
	public:
		vertex_array();
		virtual ~vertex_array();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void add_vertex_buffer(const std::shared_ptr<render::driver::buffer_interface>& vertexBuffer) override;
		virtual void remove_vertex_buffer(const std::shared_ptr<render::driver::buffer_interface>& vertexBuffer) override;
		virtual void set_index_buffer(const std::shared_ptr<render::driver::buffer_interface>& indexBuffer) override;

		const std::vector<std::shared_ptr<vertex_buffer>>& get_vertex_buffers() const { return m_VertexBuffers; }
		const std::shared_ptr<index_buffer>& get_index_buffer() const { return m_IndexBuffer; }
		uint32_t m_RendererID;
	private:
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<vertex_buffer>> m_VertexBuffers;
		std::shared_ptr<index_buffer> m_IndexBuffer;
	};
}