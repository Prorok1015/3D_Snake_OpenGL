#include "application.h"
#include <wnd_window_system.h>
#include <rnd_render_system.h>
#include <inp_input_system.h>
#include <ecs_system.h>
#include <gs_game_system.h>
#include <Windows.h>
#include <chrono>

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

	float delta_time = 1.0f / 60.0f;
	auto previous_time = std::chrono::high_resolution_clock::now();

	while (!window_system_ref.is_stop_running()) {
		auto current_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = current_time - previous_time;
		delta_time = duration.count();
		previous_time = current_time;

		wnd::get_system().pool_events();

		inp::get_system().process_input(delta_time);
		 
		ecs::process_systems(delta_time);

		gs::get_system().end_ecs_frame();

		rnd::get_system().render();
 
		window_system_ref.process_windows();
	}

	return 0;
}