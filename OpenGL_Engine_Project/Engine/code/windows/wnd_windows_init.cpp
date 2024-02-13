#include "wnd_windows_init.h"
#include "window_system.h"
#include "../game_system/gs_game_init.h"

void components::windows_init(ds::DataStorage& data)
{
	data.construct<app::WindowSystem>();
	game_init(data);
}
