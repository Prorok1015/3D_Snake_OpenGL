#include "gs_game_init.h"
#include "game_system.h"
#include "../debug_ui/debug_ui_init.h"
#include "../application.h"

void components::game_init(ds::DataStorage& data)
{
	auto& game = data.construct<app::GameSystem>();
	debug_ui_init(data);

	auto& myApp = data.require<app::Application>();

	myApp.beginFrame += [&] { game.begin_frame(); };
	myApp.capture += [&] { game.capture(); };
	myApp.render += [&] { game.render(); };
	myApp.endFrame += [&] { game.end_frame(); };
}
