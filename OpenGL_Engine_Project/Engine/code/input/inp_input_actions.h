#pragma once
#include "../common/common.h"
#include "inp_keyboard_device.h"

namespace input
{
	class InputActionBase
	{
	public:
		virtual ~InputActionBase() {};
		virtual void update(float dt) = 0;
	};

	class InputActionClick : public InputActionBase
	{
	public:
		~InputActionClick();
		template <typename HANDLER>
		static std::shared_ptr<InputActionClick> create(KEYBOARD_BUTTONS tag, HANDLER&& callback)
		{
			return std::shared_ptr<InputActionClick>(new InputActionClick(tag, callback));
		}

		virtual void update(float dt);

	private:
		template <typename HANDLER>
		InputActionClick(KEYBOARD_BUTTONS tag, HANDLER&& callback)
			: tag_(tag)
		{
			onClick += callback;
		}


	public:
		Event<void()> onClick;
		KEYBOARD_BUTTONS tag_;
		float actual_time = 0.f;

		static constexpr float activate_time = 0.2f;
	};

	class InputActionHold : public InputActionBase
	{
	public:
		~InputActionHold() = default;

		template <typename HANDLER>
		static std::shared_ptr<InputActionHold> create(KEYBOARD_BUTTONS tag, HANDLER&& callback)
		{
			return std::shared_ptr<InputActionHold>(new InputActionHold(tag, callback));
		}
		virtual void update(float dt);

	private:
		template <typename HANDLER>
		InputActionHold(KEYBOARD_BUTTONS tag, HANDLER&& callback)
			: tag_(tag)
		{
			onClick += callback;
		}

	public:
		Event<void()> onClick;
		KEYBOARD_BUTTONS tag_;
	};
}

namespace inp = input;