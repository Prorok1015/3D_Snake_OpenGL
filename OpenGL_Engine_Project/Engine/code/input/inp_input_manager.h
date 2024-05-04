#pragma once
#include "../common/common.h"
#include "inp_input_actions.h"

namespace input
{
	class InputManager
	{
		using Commands = std::vector<std::shared_ptr<inp::InputActionBase>>;
	public:
		InputManager() = default;
		~InputManager() = default;

		void notify_listeners(float dt);

		void registrate(std::shared_ptr<inp::InputActionBase> command) { get_active_commands().push_back(command); };
		void unregistrate(std::shared_ptr<inp::InputActionBase> command);

		void set_active_layer(std::string layer) { active_layer_ = layer; }
		const std::string& get_active_layer() const { return active_layer_; }

		void set_enabled(bool enable) { enabled_ = enable; }
		bool is_enabled() const { return enabled_; }

		template<typename KB, typename HANDLER>
		std::shared_ptr<InputActionBase> create_click_action(KB&& tag, HANDLER&& callback)
		{
			auto handler = InputActionClickT<KB>::create(tag, std::forward<HANDLER>(callback));
			registrate(handler);
			return handler;
		}

		template<typename KB, typename HANDLER>
		std::shared_ptr<InputActionBase> create_hold_action(KB&& tag, HANDLER&& callback)
		{
			auto handler = InputActionHoldT<KB>::create(tag, std::forward<HANDLER>(callback));
			registrate(handler);
			return handler;
		}

		template<typename HANDLER>
		std::shared_ptr<InputActionBase> create_mouse_move_action(HANDLER&& callback)
		{
			auto handler = InputActionMouseMove::create(std::forward<HANDLER>(callback));
			registrate(handler);
			return handler;
		}

	private:
		Commands& get_commands(std::string layer) { return layers_[layer]; }
		Commands& get_active_commands() { return get_commands(get_active_layer()); }

	private:
		std::unordered_map<std::string, Commands> layers_;
		std::string active_layer_ = "game";
		bool enabled_ = true;
	};

	using InputManagerRef = std::shared_ptr<InputManager>;
}

namespace inp = input;