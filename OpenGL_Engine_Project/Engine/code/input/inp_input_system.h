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
		InputSystem(const InputSystem&) = delete;
		InputSystem(InputSystem&&) = delete;
		InputSystem& operator= (const InputSystem&) = delete;
		InputSystem& operator= (InputSystem&&) = delete;

		void end_frame();

	public:
		Event<void(KEYBOARD_BUTTONS, const Key& state)> onKeyAction;

		KeyboardDevice keyboard;
		MouseDevice mouse;
	};
}

namespace inp = input;