#include "gs_game_init.h"
#include "gs_game_system.h"
#include <debug_ui_init.h>
#include <application.h>
#include <rnd_render_init.h>

extern gs::GameSystem* p_game_system;

void components::game_init(ds::AppDataStorage& data)
{
	render_init(data);
	p_game_system = &data.construct<gs::GameSystem>();
	debug_ui_init(data);

	auto& myApp = data.require<app::Application>();

	myApp.beginFrame += [] { gs::get_system().begin_frame(); }; 
}

void components::game_term(ds::AppDataStorage& data)
{
	debug_ui_term(data);
	data.destruct<gs::GameSystem>();
	render_term(data);
	p_game_system = nullptr;
}
