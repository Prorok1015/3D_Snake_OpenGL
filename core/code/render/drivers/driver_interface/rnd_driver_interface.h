#pragma once

namespace render::driver
{
	class driver_interface
	{
	public:
		virtual void set_viewport(int x, int y, int w, int h) = 0;
		virtual void set_clear_color(float r, float g, float b, float a) = 0;
		virtual void clear(int flags) = 0;
		virtual void set_activate_texture(int idx) = 0;
		virtual void set_line_size(float size) = 0;
		virtual void set_point_size(float size) = 0;
		virtual void draw_elements(unsigned int render_mode, unsigned int vao, unsigned int count) = 0;
		virtual void draw_elements(unsigned int render_mode, unsigned int count) = 0;

		virtual void enable(int flags) = 0;
	};
}