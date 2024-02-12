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

input::InputActionHold::~InputActionHold()
{
}

void input::InputActionHold::update(float dt)
{
	InputSystem* inpSys = ds::DataStorage::instance().require<InputSystem>();
	const auto& action = inpSys->keyboard.get_key(tag_);
	if (action.action == inp::KEY_ACTION::DOWN) {
		actual_time += dt;
		actual_step_time += dt;

		if (actual_time > activate_time && actual_step_time > step_time) {
			onClick();
			actual_step_time = 0.f;
		}
	}

	if (action.action == inp::KEY_ACTION::UP) {
		actual_time = 0.f;
		actual_step_time = -activate_time;
	}
}

void input::InputActionMouseMove::update(float dt)
{
	InputSystem* inpSys = ds::DataStorage::instance().require<InputSystem>();
	glm::vec2 cur_pos = inpSys->mouse.get_pos();

	if (position != cur_pos) {
		onAction(cur_pos, position);
		position = cur_pos;
	}
}
