#include "rnd_render_system.h"
#include "rnd_gl_driver.h"

rnd::RenderSystem* p_render_system = nullptr;

render::RenderSystem& render::get_system()
{
	ASSERT_MSG(p_render_system, "Render system is nullptr!");
	return *p_render_system;
}

render::RenderSystem::RenderSystem(std::unique_ptr<render::driver::driver_interface> driver)
	: pDrv(std::move(driver))
	, shManager(pDrv.get())
	, txrManager(pDrv.get())
{
	init();
}

void render::RenderSystem::init()
{
	get_sh_manager().init_global_uniform();
	get_renderer().init();
	enable(GL_DEPTH_TEST);
}

void render::RenderSystem::term()
{
	get_renderer().term();
}
