#include "application.h"
#include <windows/wnd_window_system.h>
#include "../render/rnd_render_system.h"

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
	rnd::get_system().clear_color(clear_color_);

	while (!wndCreator.is_all_windows_close()) {
		beginFrame();
		capture();

		prepair_render();
		pre_render();
		render();
		post_render();

		endFrame();
	}

	return 0;
}