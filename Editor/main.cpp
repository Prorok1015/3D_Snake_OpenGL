#include <application.h>
#include <components_init.h>
#include "code/editor_system/edt_editor_init.h"


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