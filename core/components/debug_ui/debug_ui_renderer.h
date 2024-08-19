#pragma once
#include <common.h>
#include <rnd_renderer_base.h>
#include <rnd_driver_interface.h>

namespace dbg_ui
{
	class renderer : public rnd::renderer_base
	{
	public:
		renderer();
		virtual ~renderer() override {}

		virtual void on_render(rnd::driver::driver_interface* drv);

	public:
		Event<> render_event;

	private:

	};
}