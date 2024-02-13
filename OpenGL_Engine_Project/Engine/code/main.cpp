#include "application.h"
#include "common/ds_store.hpp"
#include "windows/window_system.h"
#include "game_system/game_system.h"
#include "debug_ui/debug_ui_system.h"
#include "input/inp_input_system.h"
#include "components_init.h"

int main()
{
	com::component_init(ds::DataStorage::instance());

	app::Application& myApp = ds::DataStorage::instance().require<app::Application>();

	return myApp.run();
}