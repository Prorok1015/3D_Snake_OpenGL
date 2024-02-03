#include "application.h"
#include "common/ds_store.hpp"
#include "windows/window_system.h"

application::Application::Application()
{
}

application::Application::~Application()
{
}

int application::Application::run()
{
	auto* wndCreator = ds::DataStorage::instance().require<WindowSystem>();

	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	glEnable(GL_DEPTH_TEST);
	while (!wndCreator->is_all_windows_close()) {
		beginFrame();
		capture();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render();
		endFrame();
	}

	return 0;
}
