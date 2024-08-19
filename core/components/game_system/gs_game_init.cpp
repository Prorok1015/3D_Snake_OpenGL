#include "gs_game_init.h"
#include "gs_game_system.h"
#include <application.h>

extern gs::GameSystem* p_game_system;

void components::game_init(ds::AppDataStorage& data)
{
	p_game_system = &data.construct<gs::GameSystem>();

	auto& myApp = data.require<app::Application>();

	myApp.beginFrame += [] { gs::get_system().begin_frame(); }; 
}

void components::game_term(ds::AppDataStorage& data)
{
	data.destruct<gs::GameSystem>();
	p_game_system = nullptr;
}
