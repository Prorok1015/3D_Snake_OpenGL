#include "application.h"
#include "common/ds_store.hpp"
#include "windows/window_system.h"
#include "game_system/game_system.h"
#include "debug_ui/debug_ui_system.h"

int main()
{
	std::unique_ptr<app::WindowSystem> winSys = std::make_unique<app::WindowSystem>();
	ds::DataStorage::instance().store(winSys.get());
	std::unique_ptr<app::GameSystem> game = std::make_unique<app::GameSystem>();
	ds::DataStorage::instance().store(game.get());
	std::unique_ptr<dbg_ui::DebugUiSystem> dbgUi = std::make_unique<dbg_ui::DebugUiSystem>();
	ds::DataStorage::instance().store(dbgUi.get());

	app::Application myApp;
	myApp.beginFrame += [&] { dbgUi->BeginFrame(); game->BeginFrame(); };
	myApp.capture += [&] { dbgUi->Capture(); game->Capture(); };
	myApp.render += [&] { dbgUi->Render(); game->Render(); };
	myApp.endFrame += [&] { dbgUi->EndFrame(); game->EndFrame(); };

	return myApp.run();
}