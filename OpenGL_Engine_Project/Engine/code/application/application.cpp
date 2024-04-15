#include "application.h"
#include "../windows/window_system.h"
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

	rnd::get_system().enable(GL_DEPTH_TEST);
	while (!wndCreator.is_all_windows_close()) {
		beginFrame();
		capture();

		rnd::get_system().clear_color(clear_color_);
		rnd::get_system().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render();
		endFrame();
	}

	return 0;
}

void application::Application::set_clear_color(glm::vec4 color)
{
	clear_color_ = color;
}
