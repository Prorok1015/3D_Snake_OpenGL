#include "wnd_windows_init.h"
#include "wnd_window_system.h"
#include "../game_system/gs_game_init.h"

extern wnd::WindowSystem* p_wnd_system;

void components::windows_init(ds::AppDataStorage& data)
{
	p_wnd_system = &data.construct<wnd::WindowSystem>();
	game_init(data);
}

void components::windows_term(ds::AppDataStorage& data)
{
	game_term(data);
	data.destruct<wnd::WindowSystem>();
	p_wnd_system = nullptr;
}
