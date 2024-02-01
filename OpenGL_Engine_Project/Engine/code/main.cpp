#include "application.h"
#include "common/ds_store.hpp"
#include "windows/window_system.h"

int main()
{
	std::unique_ptr<app::WindowSystem> w = std::make_unique<app::WindowSystem>();
	ds::DataStorage::instance().store(w.get());
	app::Application myApp;
	return myApp.run();
}