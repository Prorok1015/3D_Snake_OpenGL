#include "wnd_window_init.h"
#include "wnd_window_system.h"

extern wnd::WindowSystem* p_wnd_system;

void components::window_init(ds::AppDataStorage& data)
{
	p_wnd_system = &data.construct<wnd::WindowSystem>();
}

void components::window_term(ds::AppDataStorage& data)
{

	data.destruct<wnd::WindowSystem>();
	p_wnd_system = nullptr;
}
