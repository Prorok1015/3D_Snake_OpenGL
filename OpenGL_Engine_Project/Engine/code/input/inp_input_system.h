#pragma once
#include "../common/common.h"
#include "../common/ds_type_id.hpp"
#include "inp_keyboard_device.h"
#include "inp_mouse_device.h"

namespace input
{
	class InputSystem
	{
	public:
		InputSystem();
		~InputSystem() = default;
		InputSystem(const InputSystem&) = default;
		InputSystem(InputSystem&&) = default;
		InputSystem& operator= (const InputSystem&) = default;
		InputSystem& operator= (InputSystem&&) = default;

		void end_frame();

		const Key& get_key_state(KEYBOARD_BUTTONS) const;
		const Key& get_key_state(MOUSE_BUTTONS) const;

		template<class T, class HANDLER>
		void SubscribeKeyboardByType(HANDLER&& callback)
		{
			//onKeyAction.Subscribe(ds::Type::value<T>(), std::forward<HANDLER>(callback));
			onKeyAction += callback;
		}

		template<class T>
		void UnsubscribeKeyboardByType()
		{
			//onKeyAction.Unsubscribe(ds::Type::value<T>());
		}

		MouseDevice mouse;
		KeyboardDevice keyboard;
	private:
		Event<void(KEYBOARD_BUTTONS keycode, KEY_ACTION action)> onKeyAction;

	};
}

namespace inp = input;