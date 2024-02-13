#include "application.h"
#include "common/ds_store.hpp"
#include "windows/window_system.h"
#include "game_system/game_system.h"
#include "debug_ui/debug_ui_system.h"
#include "input/inp_input_system.h"

int main()
{
	auto& inpSys = ds::DataStorage::instance().construct<inp::InputSystem>();
	auto& winSys = ds::DataStorage::instance().construct<app::WindowSystem>();
	auto& game = ds::DataStorage::instance().construct<app::GameSystem>();
	auto& dbgUi = ds::DataStorage::instance().construct<dbg_ui::DebugUiSystem>();

	app::Application myApp;
	myApp.beginFrame += [&] { dbgUi.begin_frame(); game.begin_frame(); };
	myApp.capture += [&] { dbgUi.capture(); game.capture(); };
	myApp.render += [&] { dbgUi.render(); game.render(); };
	myApp.endFrame += [&] { dbgUi.end_frame(); game.end_frame(); inpSys.end_frame(); };

	return myApp.run();
}