#include "gs_game_init.h"
#include "game_system.h"
#include "../debug_ui/debug_ui_init.h"
#include "../application.h"

extern gm::GameSystem* p_game_system;

void components::game_init(ds::AppDataStorage& data)
{
	p_game_system = &data.construct<gm::GameSystem>();
	debug_ui_init(data);

	auto& myApp = data.require<app::Application>();

	myApp.beginFrame += [] { gm::get_system().begin_frame(); };
	myApp.capture += [] { gm::get_system().capture(); };
	myApp.render += [] { gm::get_system().render(); };
	myApp.endFrame += [] { gm::get_system().end_frame(); };
}

void components::game_term(ds::AppDataStorage& data)
{
	debug_ui_term(data);

	data.destruct<gm::GameSystem>();
	p_game_system = nullptr;
}
