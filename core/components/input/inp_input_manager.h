#pragma once
#include <common.h>
#include "inp_input_actions.h"
#include "inp_input_manager_base.h"

namespace inp
{
	class InputManager : public input_manager_base
	{
		using Commands = std::vector<std::shared_ptr<inp::InputActionBase>>;
	public:
		InputManager() = default;

		virtual ~InputManager() override {};
		virtual void on_notify_listeners(float dt) override
		{
			notify_listeners(dt);
		}

		void notify_listeners(float dt);

		void registrate(std::shared_ptr<inp::InputActionBase> command) { get_active_commands().push_back(command); };
		void unregistrate(std::shared_ptr<inp::InputActionBase> command);

		void set_active_layer(std::string layer) { active_layer_name = layer; }
		const std::string& get_active_layer() const { return active_layer_name; }

		void set_enabled(bool enable) { is_enabled = enable; }
		bool get_is_enabled() const { return is_enabled; }

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
		Commands& get_commands(std::string layer_name) { return layers[layer_name]; }
		Commands& get_active_commands() { return get_commands(get_active_layer()); }

	private:
		std::unordered_map<std::string, Commands> layers;
		std::string active_layer_name = "game";
		bool is_enabled = true;
	};

	using InputManagerRef = std::shared_ptr<InputManager>;
}
 