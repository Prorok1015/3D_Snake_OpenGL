#pragma once
#include <rnd_driver_interface.h>

namespace render::driver::gl
{
	class driver : public render::driver::driver_interface
	{
	public:
		virtual void set_viewport(glm::ivec4 rect) override;
		virtual void set_clear_color(glm::vec4 color) override;
		virtual void clear(CLEAR_FLAGS flags) override;
		virtual void set_activate_texture(int idx) override;
		virtual void set_line_size(float size) override;
		virtual void set_point_size(float size) override;
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int vao, unsigned int count) override;
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int count) override;

		virtual void enable(ENABLE_FLAGS flags) override;

		virtual void unuse() override;

		virtual std::unique_ptr<shader_interface> create_shader(const std::vector<shader_header>& headers) override;
		virtual std::unique_ptr<texture_interface> create_texture(const texture_header& header) override;
		virtual std::unique_ptr<vertex_array_interface> create_vertex_array() override;
		virtual std::unique_ptr<buffer_interface> create_buffer() override;
		virtual std::unique_ptr<uniform_buffer_interface> create_uniform_buffer(std::size_t size, std::size_t binding) override;
	};
}