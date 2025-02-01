#pragma once
#include <rnd_driver_interface.h>
#include <stack>
#include <glad/glad.h>

namespace rnd::driver::gl
{
	class driver : public rnd::driver::driver_interface
	{
	public:
		driver();
		virtual ~driver() override;

		void attach_frame_buffer();
		void dettach_frame_buffer();
		virtual void push_frame_buffer() override;
		virtual void pop_frame_buffer() override;
		virtual void set_render_rarget(texture_interface* color, texture_interface* depth_stencil = nullptr) override;

		virtual void set_viewport(glm::ivec4 rect) override;
		virtual void set_clear_color(glm::vec4 color) override;
		virtual void clear(CLEAR_FLAGS flags) override;
		virtual void clear(CLEAR_FLAGS flags, glm::vec4 color) override;
		virtual void set_activate_texture(int idx) override;
		virtual void set_line_size(float size) override;
		virtual void set_point_size(float size) override;
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int vao, unsigned int count) override;
		virtual void draw_indeces(const std::unique_ptr<vertex_array_interface>& verteces, RENDER_MODE render_mode, unsigned int count, unsigned int offset = 0) override;
		virtual void draw_instanced_indeces(const std::unique_ptr<vertex_array_interface>& verteces, RENDER_MODE render_mode, unsigned int count, unsigned int instance_count, unsigned int offset = 0) override;

		virtual void enable(ENABLE_FLAGS flags) override;
		virtual void disable(ENABLE_FLAGS flags) override;

		virtual void unuse() override;

		virtual std::unique_ptr<shader_interface> create_shader(const std::vector<shader_header>& headers) override;
		virtual std::unique_ptr<texture_interface> create_texture(const texture_header& header) override;
		virtual std::unique_ptr<texture_interface> create_texture(const cubmap_texture_header& headers) override;
		virtual std::unique_ptr<vertex_array_interface> create_vertex_array() override;
		virtual std::unique_ptr<buffer_interface> create_buffer() override;
		virtual std::unique_ptr<uniform_buffer_interface> create_uniform_buffer(std::size_t size, std::size_t binding) override;
	private:
		glm::ivec4 viewport{ 0 };
		std::stack<std::pair<GLuint, GLuint>> framebuffers;
	};
}