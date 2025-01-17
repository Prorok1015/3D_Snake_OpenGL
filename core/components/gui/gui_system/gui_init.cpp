#include "gui_init.h"
#include "gui_system.h"

extern gui::gui_system* p_gui_system;

void components::gui_init(ds::AppDataStorage& data)
{
	p_gui_system = &data.construct<gui::gui_system>();
}

void components::gui_term(ds::AppDataStorage& data)
{
	data.destruct<gui::gui_system>();
	p_gui_system = nullptr;
}
