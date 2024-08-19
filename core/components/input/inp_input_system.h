#pragma once
#include <common.h>
#include <ds_type_id.hpp>
#include "inp_keyboard_device.h"
#include "inp_mouse_device.h"

namespace input
{
	class InputSystem
	{
	public:
		InputSystem();
		~InputSystem() = default;
		InputSystem(InputSystem&&) = default;
		InputSystem& operator= (InputSystem&&) = default;

		InputSystem(const InputSystem&) = delete;
		InputSystem& operator= (const InputSystem&) = delete;

		// change
		void end_frame();

		Key get_key_state(KEYBOARD_BUTTONS) const;
		Key get_key_state(MOUSE_BUTTONS) const;

		template<class T, class HANDLER>
		void sub_keyboard_by_tag(HANDLER&& callback)
		{
			onKeyAction += callback;
		}

		template<class T>
		void unsub_keyboard_by_tag()
		{
			//onKeyAction.Unsubscribe(ds::Type::value<T>());
		}

		MouseDevice mouse;
		KeyboardDevice keyboard;
	private:
		Event<void(KEYBOARD_BUTTONS keycode, KEY_ACTION action)> onKeyAction;

	};

	InputSystem& get_system();
}

namespace inp = input;