#include "components_init.h"
#include "../common/common.h"
#include "application.h"
#include "../input/inp_input_init.h"

extern app::Application* p_app_system;

void components::component_init(ds::AppDataStorage& data)
{
	p_app_system = &data.construct<app::Application>();
	input_init(data);
}

void components::component_term(ds::AppDataStorage& data)
{
	input_term(data);
	data.destruct<app::Application>();
	p_app_system = nullptr;
}
