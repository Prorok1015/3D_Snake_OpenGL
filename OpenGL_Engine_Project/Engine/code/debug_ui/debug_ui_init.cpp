#include "debug_ui_init.h"
#include "debug_ui_system.h"
#include "../application.h"

void components::debug_ui_init(ds::DataStorage& data)
{
	auto& dbgUi = data.construct<dbg_ui::DebugUiSystem>();
	auto& myApp = data.require<app::Application>();
	myApp.beginFrame += [&] { dbgUi.begin_frame(); };
	myApp.capture += [&] { dbgUi.capture(); };
	myApp.render += [&] { dbgUi.render(); };
	myApp.endFrame += [&] { dbgUi.end_frame(); };

}
