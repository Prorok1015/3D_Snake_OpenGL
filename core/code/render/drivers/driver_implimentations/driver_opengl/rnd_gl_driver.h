#pragma once
#include <rnd_driver_interface.h>

namespace render::driver::gl
{
	class driver : public render::driver::driver_interface
	{
	public:
		virtual void set_viewport(int x, int y, int w, int h) override;
		virtual void set_clear_color(float r, float g, float b, float a) override;
		virtual void clear(int flags) override;
		virtual void set_activate_texture(int idx) override;
		virtual void set_line_size(float size) override;
		virtual void set_point_size(float size) override;
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int vao, unsigned int count) override;
		virtual void draw_elements(RENDER_MODE render_mode, unsigned int count) override;

		virtual void enable(int flags) override;
	};
}