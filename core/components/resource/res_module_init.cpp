#include "res_module_init.h"
#include "res_system.h"

extern res::ResourceSystem* p_res_system;

void components::resource_init(ds::AppDataStorage& data)
{
	p_res_system = &data.construct<res::ResourceSystem>();
}

void components::resource_term(ds::AppDataStorage& data)
{
	data.destruct<res::ResourceSystem>();
	p_res_system = nullptr;
}