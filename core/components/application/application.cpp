#include "application.h"
#include <wnd_window_system.h>
#include <rnd_render_system.h>
#include <inp_input_system.h>

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
	auto& window_system_ref = wnd::get_system();
	//TODO move to renderer
	rnd::get_system().clear_color(clear_color_);

	window_system_ref.init_windows_frame_time();

	while (!window_system_ref.is_stop_running()) {
		//TODO move to input system
		inp::get_system().process_input(window_system_ref.get_active_window()->get_delta());
		 
		rnd::get_system().produce_renderers();
 
		window_system_ref.produce_windows();
	}

	return 0;
}