#include "application.h"
#include <wnd_window_system.h>
#include <rnd_render_system.h>
#include <inp_input_system.h>
#include <ecs/ecs_system.h>
#include <gs_game_system.h>
#include <Windows.h>

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

	window_system_ref.init_windows_frame_time();
	long long time = GetTickCount(); // todo
	while (!window_system_ref.is_stop_running()) {
		inp::get_system().process_input(window_system_ref.get_active_window()->get_delta());
		 
		ecs::process_systems(float(GetTickCount() - time) / 1000.f);

		gs::get_system().end_ecs_frame();

		rnd::get_system().process_renderers();
 
		window_system_ref.process_windows();
	}

	return 0;
}