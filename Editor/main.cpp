#include <application.h>
#include <components_init.h>
#include "code/editor_system/edt_editor_init.h"

#ifndef _WINDOWS
#include "Windows.h"

int WINAPI WinMain(HINSTANCE /*hInstance*/,     // The instance
	HINSTANCE /*hPrevInstance*/,  // Previous instance
	LPSTR /*lpCmdLine*/,          // Command Line Parameters
	int /*nShowCmd*/)            // Window Show State
{
	ds::AppDataStorage app_storage;
	com::component_init(app_storage);
	com::editor_init(app_storage);

	app::Application& myApp = app::get_app_system();
	int result = myApp.run();

	com::editor_term(app_storage);
	com::component_term(app_storage);

	return result;
}

#else

int main(int argc, char* args[])
{
	ds::AppDataStorage app_storage;
	com::component_init(app_storage);
	com::editor_init(app_storage);

	app::Application& myApp = app::get_app_system();
	int result = myApp.run();

	com::editor_term(app_storage);
	com::component_term(app_storage);

	return result;
}

#endif