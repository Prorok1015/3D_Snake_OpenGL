#pragma once
#include <common.h>
#include "inp_input_actions.h"
#include "inp_input_manager_base.h"

namespace inp
{
	class input_manager : public input_manager_base
	{
		using Commands = std::vector<std::shared_ptr<inp::InputActionBase>>;
	public:
		input_manager()
			: input_manager_base("editor", 0) {}

		virtual ~input_manager() override {};
		virtual void on_notify_listeners(float dt) override
		{
			notify_listeners(dt);
		}

		virtual bool on_handle_event(const input_event&) override;

		void notify_listeners(float dt);
		
		// there is unblock input to next levels for one frame.
		// by default input manager is blocking 
		// keyboard_event, mouse_click_event with KEY_ACTION::DOWN and scroll_move_event
		void unblock_layer_once();
		void block_layer();
		void unblock_layer();

		void registrate(std::shared_ptr<inp::InputActionBase> command) { get_active_commands().push_back(command); };
		void unregistrate(std::shared_ptr<inp::InputActionBase> command);

		void set_active_layer(std::string layer) { active_layer_name = layer; }
		const std::string& get_active_layer() const { return active_layer_name; }

		void set_enabled(bool enable) { is_enabled = enable; }
		bool get_is_enabled() const { return is_enabled; }

		template<typename KB, typename HANDLER>
		std::shared_ptr<InputActionBase> create_action(KB&& tag, HANDLER&& callback)
		{
			auto handler = InputAction<KB>::create(tag, std::forward<HANDLER>(callback));
			registrate(handler);
			return handler;
		}

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

		template<typename HANDLER>
		std::shared_ptr<InputActionBase> create_mouse_scroll_action(HANDLER&& callback)
		{
			auto handler = InputMouseWhell::create(std::forward<HANDLER>(callback));
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
		bool is_block_keyaction_down = true;
		bool is_block_keyaction_down_once = true;
	};

	using InputManagerRef = std::shared_ptr<input_manager>;
}
 