#pragma once
#include "../common/common.h"
#include "inp_key_enums.hpp"

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
		~InputActionHold();
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
		float actual_time = 0.f;
		float actual_step_time = -activate_time;
		static constexpr float step_time = 0.f;
		static constexpr float activate_time = 0.2f;
	};

	class InputActionMouseMove : public InputActionBase
	{
	public:
		~InputActionMouseMove() = default;
		template <typename HANDLER>
		static std::shared_ptr<InputActionMouseMove> create(HANDLER&& callback)
		{
			return std::shared_ptr<InputActionMouseMove>(new InputActionMouseMove(callback));
		}

		virtual void update(float dt);

	private:
		template <typename HANDLER>
		InputActionMouseMove(HANDLER&& callback) {
			onAction += callback;
		}

	private:
		Event<void(glm::vec2, glm::vec2)> onAction;
		glm::vec2 position {0.f};
	};
}

namespace inp = input;