#include "application.h"
#include "common/ds_store.hpp"
#include "components_init.h"

int main()
{
	com::component_init(ds::DataStorage::instance());

	app::Application& myApp = ds::DataStorage::instance().require<app::Application>();

	return myApp.run();
}