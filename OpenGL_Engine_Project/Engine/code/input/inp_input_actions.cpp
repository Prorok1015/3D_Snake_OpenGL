#include "inp_input_actions.h"
#include "inp_input_system.h"
#include "../common/ds_store.hpp"
#include "../windows/window.h"

input::InputActionClick::~InputActionClick()
{
}

void input::InputActionClick::update(float dt)
{
	InputSystem* inpSys = ds::DataStorage::instance().require<InputSystem>();
	const auto& action = inpSys->keyboard.get_key(tag_);

	if (action.action == inp::KEY_ACTION::DOWN) {
		actual_time += dt;
	}

	if (action.action == inp::KEY_ACTION::UP) {
		if (actual_time > 0.f && actual_time < activate_time) {
			onClick();
			actual_time = 0.f;
		}
	}
}

void input::InputActionHold::update(float dt)
{
	InputSystem* inpSys = ds::DataStorage::instance().require<InputSystem>();
	const auto& action = inpSys->keyboard.get_key(tag_);
	bool isHOLD =	action.action == inp::KEY_ACTION::DOWN &&
					action.time_stamp_down + 0.2f < app::Window::current_time();

	if (isHOLD) {
		onClick();
	}
}

