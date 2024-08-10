#pragma once
#include <string>
#include <vector>
#include <memory>

namespace render::driver
{
	class shader_interface;
	class texture_interface;
	class render_context_interface;

	struct shader_header
	{
		enum class TYPE { VERTEX, FRAGMENT, GEOM};

		std::string body;
		TYPE type;
	};

	struct texture_header
	{
		enum class FILTERING
		{
			NEAREST,
			LINEAR,
			LINEAR_MIPMAP
		};

		enum class WRAPPING
		{
			REPEAT,
			REPEAT_MIRROR,
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER
		};

		FILTERING min;
		FILTERING mag;
		WRAPPING wrap;
		int width;
		int height;
		int channels;
		unsigned char* data;
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
		virtual void set_viewport(int x, int y, int w, int h) = 0;
		virtual void set_clear_color(float r, float g, float b, float a) = 0;
		virtual void clear(int flags) = 0;
		virtual void set_activate_texture(int idx) = 0;
		virtual void set_line_size(float size) = 0;
		virtual void set_point_size(float size) = 0;
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int vao, unsigned int count) = 0;
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int count) = 0;

		virtual void enable(int flags) = 0;

		virtual void unuse() = 0;

		virtual std::unique_ptr<shader_interface> create_shader(const std::vector<shader_header>& headers) = 0;
		virtual std::unique_ptr<texture_interface> create_texture(const texture_header& headers) = 0;
	};
}