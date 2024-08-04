#pragma once
#include <rnd_buffer_interface.h>
#include <glad/glad.h>
#include <vector>
#include <rnd_gl_buffer_layout.h>

namespace render::driver::gl
{
	class vertex_buffer : public render::driver::buffer_interface
	{
	public:
		vertex_buffer(std::size_t size);
		vertex_buffer(float* vertices, std::size_t size);
		~vertex_buffer();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void set_data(const void* data, std::size_t size) override;

		template<class T>
		void set_data(const std::vector<T>& data) {
			set_data(data.data(), data.size() * sizeof(T));
		}

		const BufferLayout& get_layout() const { return m_Layout; }
		void set_layout(const BufferLayout& layout) { m_Layout = layout; }
	private:
		GLuint m_RendererID = 0;
		BufferLayout m_Layout;

	};

	class index_buffer : public render::driver::buffer_interface
	{
	public:
		index_buffer(const unsigned int* indices, std::size_t count);
		template<class T>
		index_buffer(const std::vector<T>& indices)
			:index_buffer(indices.data(), indices.size()) {}
		virtual ~index_buffer();

		virtual void bind() override;
		virtual void unbind() override;
		virtual void set_data(const void* data, std::size_t size) override {};

		std::size_t get_count() const { return m_Count; }
	private:
		GLuint m_RendererID = 0;
		std::size_t m_Count = 0;
	};
}