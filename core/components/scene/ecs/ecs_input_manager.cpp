#include "ecs_input_manager.h"
#include "ecs_common_system.h"
#include "inp_input_system.h"
#include "ecs_component.h"

void ecs::flow_input_manager::on_notify_listeners(float dt)
{
	auto& keyboard = inp::get_system().keyboard;
	auto& mouse = inp::get_system().mouse;
	bool was_changed = false;

	if (keyboard != keyboard_prev)
	{
		keyboard_prev = keyboard;
		was_changed = true;
	}

	if (mouse != mouse_prev)
	{
		mouse_prev = mouse;
		was_changed = true;
	}

	if (was_changed) {
		ecs::add_component(input_ent, input_changed_event_component{});
	}
}
