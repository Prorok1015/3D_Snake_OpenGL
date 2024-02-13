#pragma once
#include "inp_keyboard_device.h"
#include "inp_mouse_device.h"

namespace input
{
	class InputSystem
	{
	public:
		InputSystem() = default;
		~InputSystem() = default;
		InputSystem(const InputSystem&) = default;
		InputSystem(InputSystem&&) = default;
		InputSystem& operator= (const InputSystem&) = default;
		InputSystem& operator= (InputSystem&&) = default;

		void end_frame();

		const Key& get_key_state(KEYBOARD_BUTTONS) const;
		const Key& get_key_state(MOUSE_BUTTONS) const;

	public:
		Event<void(KEYBOARD_BUTTONS, const Key& state)> onKeyAction;

		KeyboardDevice keyboard;
		MouseDevice mouse;
	};
}

namespace inp = input;