#include "debug_ui_init.h"
#include "debug_ui_system.h"
#include "../application.h"

extern dbg_ui::DebugUiSystem* p_dbgui_system;

void components::debug_ui_init(ds::AppDataStorage& data)
{
	p_dbgui_system = &data.construct<dbg_ui::DebugUiSystem>();
	auto& myApp = data.require<app::Application>();
	myApp.beginFrame += [] { dbg_ui::get_system().begin_frame(); };
	myApp.capture += [] { dbg_ui::get_system().capture(); };
	myApp.render += [] { dbg_ui::get_system().render(); };
	myApp.endFrame += [] { dbg_ui::get_system().end_frame(); };

}

void components::debug_ui_term(ds::AppDataStorage& data)
{
	data.destruct<dbg_ui::DebugUiSystem>();
	p_dbgui_system = nullptr;
}
