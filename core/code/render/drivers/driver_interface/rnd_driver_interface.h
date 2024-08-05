#pragma once

namespace render::driver
{
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
	};
}