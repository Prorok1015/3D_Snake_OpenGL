#include "debug_ui_init.h"
#include "debug_ui_system.h"
#include <application.h>

extern dbg_ui::DebugUiSystem* p_dbgui_system;

void components::debug_ui_init(ds::AppDataStorage& data)
{
	p_dbgui_system = &data.construct<dbg_ui::DebugUiSystem>();  
}

void components::debug_ui_term(ds::AppDataStorage& data)
{
	data.destruct<dbg_ui::DebugUiSystem>();
	p_dbgui_system = nullptr;
}
