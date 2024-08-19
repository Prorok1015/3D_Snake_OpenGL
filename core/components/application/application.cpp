#include "application.h"
#include <wnd_window_system.h>
#include <rnd_render_system.h>

app::Application* p_app_system = nullptr;

application::Application& application::get_app_system()
{
	ASSERT_MSG(p_app_system, "Application system is nullptr!");
	return *p_app_system;
}

application::Application::Application()
{
}

application::Application::~Application()
{
}

int application::Application::run()
{
	auto& wndCreator = wnd::get_system();
	//TODO move to renderer
	rnd::get_system().clear_color(clear_color_);

	wndCreator.init_all_windows_frame();

	while (!wndCreator.is_stop_running()) {
		//TODO move to input system
		beginFrame();
		rnd::get_system().produce_renderers();
		endFrame();

		wndCreator.produce_windows();
	}

	return 0;
}