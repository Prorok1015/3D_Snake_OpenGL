#pragma once
#include "../common/common.h"
#include "inp_key_enums.hpp"

#include "inp_input_system.h"
#include "../common/ds_store.hpp"

namespace input
{
	class InputActionBase
	{
	public:
		virtual ~InputActionBase() {};
		virtual void update(float dt) = 0;
	};

	template<typename KEY_BUTTON>
	class InputActionClickT : public InputActionBase
	{
	public:
		~InputActionClickT() = default;
		template <typename KB, typename HANDLER>
		static auto create(KB tag, HANDLER&& callback)
		{
			return std::shared_ptr<InputActionClickT<KB>>(new InputActionClickT<KB>(tag, std::move(callback)));
		}

		virtual void update(float dt)
		{
			InputSystem& inpSys = ds::DataStorage::instance().require<InputSystem>();
			const auto& action = inpSys.get_key_state(tag_);

			if (action.action == inp::KEY_ACTION::DOWN) {
				actual_time += dt;
			}

			if (action.action == inp::KEY_ACTION::UP) {
				if (actual_time > 0.f && actual_time < activate_time) {
					onAction();
				}
				actual_time = 0.f;
			}
		}

	private:
		template <typename HANDLER>
		InputActionClickT(KEY_BUTTON tag, HANDLER&& callback)
			: tag_(tag)
		{
			onAction += callback;
		}


	public:
		Event<void()> onAction;
		KEY_BUTTON tag_;
		float actual_time = 0.f;

		static constexpr float activate_time = 0.2f;
	};

	template<typename KEY_BUTTON>
	class InputActionHoldT : public InputActionBase
	{
	public:
		~InputActionHoldT() = default;
		template <typename KB, typename HANDLER>
		static auto create(KB tag, HANDLER&& callback)
		{
			return std::shared_ptr<InputActionHoldT<KB>>(new InputActionHoldT<KB>(tag, callback));
		}

		virtual void update(float dt)
		{
			InputSystem& inpSys = ds::DataStorage::instance().require<InputSystem>();
			const auto& action = inpSys.keyboard.get_key(tag_);
			if (action.action == inp::KEY_ACTION::DOWN) {
				actual_time += dt;
				actual_step_time += dt;

				if (actual_time > activate_time && actual_step_time > step_time) {
					onAction();
					actual_step_time = 0.f;
				}
			}

			if (action.action == inp::KEY_ACTION::UP) {
				actual_time = 0.f;
				actual_step_time = -activate_time;
			}
		}

	private:
		template <typename HANDLER>
		InputActionHoldT(KEY_BUTTON tag, HANDLER&& callback)
			: tag_(tag)
		{
			onAction += callback;
		}


	public:
		Event<void()> onAction;
		KEY_BUTTON tag_;
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
			return std::shared_ptr<InputActionMouseMove>(new InputActionMouseMove(std::move(callback)));
		}

		virtual void update(float dt);

	private:
		template <typename HANDLER>
		InputActionMouseMove(HANDLER&& callback) {
			onAction += callback;
		}

	private:
		Event<void(glm::vec2, glm::vec2)> onAction;
		glm::vec2 position{ 0.f };
	};
}

namespace inp = input;