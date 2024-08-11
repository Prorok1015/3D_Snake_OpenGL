#include <application.h>
#include <components_init.h>

int main()
{
	ds::AppDataStorage app_storage;
	com::component_init(app_storage);

	app::Application& myApp = app::get_app_system();
	int result = myApp.run();

	com::component_term(app_storage);

	return result;
}