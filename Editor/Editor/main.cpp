#include <engine/application/application.h>
#include <engine/application/components_init.h>
#include "code/editor/edt_editor_init.h"

int main()
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