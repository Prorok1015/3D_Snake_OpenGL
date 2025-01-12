#include "debug_ui_renderer.h"

dbg_ui::renderer::renderer(gui::imgui_backend_interface* backend_)
	: rnd::renderer_base(0)
	, backend(backend_)
{
}

void dbg_ui::renderer::on_render(rnd::driver::driver_interface* drv)
{
	backend->new_frame();
	render_event();
	backend->render();
}
