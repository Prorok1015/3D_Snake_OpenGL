#include "components_init.h"
#include "application.h"
#include <inp_input_init.h>
#include <res_resource_init.h>
#include <rnd_render_init.h>
#include <gs_game_init.h>
#include <debug_ui_init.h>
#include <wnd_window_init.h>

extern app::Application* p_app_system;

void components::component_init(ds::AppDataStorage& data)
{
	p_app_system = &data.construct<app::Application>();
	//1
	resource_init(data);
	input_init(data);
	//2
	window_init(data);
	//3
	render_init(data);
	debug_ui_init(data);
	game_init(data);
}

void components::component_term(ds::AppDataStorage& data)
{
	game_term(data);
	debug_ui_term(data);
	render_term(data);

	window_term(data);

	input_term(data);
	resource_term(data);

	data.destruct<app::Application>();
	p_app_system = nullptr;
}
