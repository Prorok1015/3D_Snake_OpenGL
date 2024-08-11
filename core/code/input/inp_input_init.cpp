#include "inp_input_init.h"
#include "inp_input_system.h"
#include <application.h>
#include <wnd_windows_init.h>

extern inp::InputSystem* p_inp_system;

void components::input_init(ds::AppDataStorage& data)
{
	p_inp_system = &data.construct<inp::InputSystem>();
	auto& myApp = data.require<app::Application>();

	myApp.endFrame += [] { inp::get_system().end_frame(); };
	windows_init(data);
}

void components::input_term(ds::AppDataStorage& data)
{
	windows_term(data);
	data.destruct<inp::InputSystem>();
	p_inp_system = nullptr;
}
