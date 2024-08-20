#include "gs_game_init.h"
#include "gs_game_system.h"
#include <application.h>

extern gs::GameSystem* p_game_system;

void components::game_init(ds::AppDataStorage& data)
{
	p_game_system = &data.construct<gs::GameSystem>();
}

void components::game_term(ds::AppDataStorage& data)
{
	data.destruct<gs::GameSystem>();
	p_game_system = nullptr;
}
