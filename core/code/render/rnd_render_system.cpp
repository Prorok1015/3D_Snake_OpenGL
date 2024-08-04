#include "rnd_render_system.h"
#include "rnd_gl_driver.h"

rnd::RenderSystem* p_render_system = nullptr;

render::RenderSystem& render::get_system()
{
	ASSERT_MSG(p_render_system, "Render system is nullptr!");
	return *p_render_system;
}

render::RenderSystem::RenderSystem()
{
	pDrv = std::make_unique<render::driver::gl::driver>();
}

void render::RenderSystem::init()
{
	get_sh_manager().init_global_uniform();
	get_renderer().init();
}

void render::RenderSystem::term()
{
	get_renderer().term();
}
