#include "inp_input_init.h"
#include "inp_input_system.h"
#include <application.h>

extern inp::InputSystem* p_inp_system;

void components::input_init(ds::AppDataStorage& data)
{
	p_inp_system = &data.construct<inp::InputSystem>();
}

void components::input_term(ds::AppDataStorage& data)
{
	data.destruct<inp::InputSystem>();
	p_inp_system = nullptr;
}
