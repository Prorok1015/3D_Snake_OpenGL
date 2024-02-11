#pragma once
#include "inp_keyboard_device.h"

namespace input
{
	class InputSystem
	{
	public:
		void end_frame();

		Event<void(KEYBOARD_BUTTONS, const Key& state)> onKeyAction;

		KeyboardDevice keyboard;
	};
}

namespace inp = input;