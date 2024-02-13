#include "inp_input_init.h"
#include "inp_input_system.h"
#include "../application.h"
#include "../windows/wnd_windows_init.h"

void components::input_init(ds::DataStorage& data)
{
	auto& inp = data.construct<inp::InputSystem>();
	auto& myApp = data.require<app::Application>();

	myApp.endFrame += [&] { inp.end_frame(); };
	windows_init(data);
}
