#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace rnd::driver
{
	class shader_interface;
	class texture_interface;
	struct texture_header;
	struct cubmap_texture_header;
	class cubemap_interface;
	class render_context_interface;
	class vertex_array_interface;
	class uniform_buffer_interface;
	class buffer_interface;

	enum class CLEAR_FLAGS { COLOR_BUFFER, DEPTH_BUFFER	};
	enum class ENABLE_FLAGS { DEPTH_TEST, DEPTH_TEST_LEQUEL, DEPTH_TEST_EQUEL, FACE_CULLING, DEPTH_MASK, COLOR_TEST };

	struct shader_header
	{
		enum class TYPE { VERTEX, FRAGMENT, GEOM };
		std::string title;
		std::string body;
		TYPE type;
	};

	enum class RENDER_MODE
	{
		TRIANGLE,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		TRIANGLE_ADJ,
		TRIANGLE_STRIP_ADJ,
		LINE,
		LINE_STRIP,
		LINE_LOOP,
		LINE_ADJ,
		LINE_STRIP_ADJ,
		POINT,
	};

	class driver_interface
	{
	public:
		virtual ~driver_interface() {}

		virtual void push_frame_buffer() = 0;
		virtual void pop_frame_buffer() = 0;
		virtual void set_render_rarget(texture_interface* color, texture_interface* depth_stencil = nullptr) = 0;

		virtual void set_viewport(glm::ivec4 rect) = 0;
		virtual void set_clear_color(glm::vec4 color) = 0;
		virtual void clear(CLEAR_FLAGS flags) = 0;
		virtual void clear(CLEAR_FLAGS flags, glm::vec4 color) = 0;
		virtual void set_activate_texture(int idx) = 0;
		virtual void set_line_size(float size) = 0;
		virtual void set_point_size(float size) = 0;
		//TODO: modify
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int vao, unsigned int count) = 0;
		virtual void draw_indeces(const std::unique_ptr<vertex_array_interface>& verteces, RENDER_MODE render_mode, unsigned int count, unsigned int offset = 0) = 0;
		virtual void draw_instanced_indeces(const std::unique_ptr<vertex_array_interface>& verteces, RENDER_MODE render_mode, unsigned int count, unsigned int instance_count, unsigned int offset = 0) = 0;

		//TODO: change to barrier abstraction
		virtual void enable(ENABLE_FLAGS flags) = 0;
		virtual void disable(ENABLE_FLAGS flags) = 0;

		//TODO: remove
		virtual void unuse() = 0;

		virtual std::unique_ptr<shader_interface> create_shader(const std::vector<shader_header>& headers) = 0;
		virtual std::unique_ptr<texture_interface> create_texture(const texture_header& headers) = 0;
		virtual std::unique_ptr<texture_interface> create_texture(const cubmap_texture_header& headers) = 0;
		virtual std::unique_ptr<vertex_array_interface> create_vertex_array() = 0;
		virtual std::unique_ptr<uniform_buffer_interface> create_uniform_buffer(std::size_t size, std::size_t binding) = 0;
		virtual std::unique_ptr<buffer_interface> create_buffer() = 0;
	};
}