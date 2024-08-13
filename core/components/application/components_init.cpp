#include "components_init.h"
#include "application.h"
#include "inp_input_init.h"
#include "res_resource_init.h"
#include "rnd_render_init.h"

extern app::Application* p_app_system;

void components::component_init(ds::AppDataStorage& data)
{
	p_app_system = &data.construct<app::Application>();
	resource_init(data);
	input_init(data);
}

void components::component_term(ds::AppDataStorage& data)
{
	input_term(data);
	resource_term(data);
	data.destruct<app::Application>();
	p_app_system = nullptr;
}
