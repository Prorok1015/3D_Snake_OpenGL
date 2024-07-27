#include "rnd_render_init.h"
#include "rnd_render_system.h"
#include "../application/application.h"
#include "../windows/wnd_windows_init.h"

extern rnd::RenderSystem* p_render_system;

void components::render_init(ds::AppDataStorage& data)
{
	p_render_system = &data.construct<rnd::RenderSystem>();
}

void components::render_term(ds::AppDataStorage& data)
{
	data.destruct<rnd::RenderSystem>();
	p_render_system = nullptr;
}