#pragma once
#include <common.h>
#include <ds_type_id.hpp>
#include "inp_keyboard_device.h"
#include "inp_mouse_device.h"
#include "inp_input_manager_base.h"

namespace inp
{
	class InputSystem
	{
	public:
		InputSystem();
		~InputSystem();
		InputSystem(InputSystem&&) = delete;
		InputSystem& operator= (InputSystem&&) = delete;

		InputSystem(const InputSystem&) = delete;
		InputSystem& operator= (const InputSystem&) = delete;

		void process_input(float dt);

		void activate_manager(std::weak_ptr<input_manager_base> inp_manager);
		void deactivate_manager(std::weak_ptr<input_manager_base> inp_manager);

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

		std::vector<std::weak_ptr<input_manager_base>> input_managers_list;
	};

	InputSystem& get_system();
}
 