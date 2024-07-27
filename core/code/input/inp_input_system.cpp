#include "inp_input_system.h"

input::InputSystem* p_inp_system = nullptr;

input::InputSystem& input::get_system()
{
	ASSERT_MSG(p_inp_system, "Input system is nullptr!");
	return *p_inp_system;
}

input::InputSystem::InputSystem()
{
	keyboard.onKeyStateChanged += [this](auto a, auto b) { onKeyAction(a, b); };
}

void input::InputSystem::end_frame()
{
	glfwPollEvents();

	//keyboard.visit_keys(onKeyAction);
}

input::Key input::InputSystem::get_key_state(KEYBOARD_BUTTONS key) const
{
	return keyboard.get_key(key);
}

input::Key input::InputSystem::get_key_state(MOUSE_BUTTONS key) const
{
	return mouse.get_key(key);
}
