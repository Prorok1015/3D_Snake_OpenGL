#pragma once
#include "common.h"
#include "inp_input_actions.h"
#include "inp_input_manager.h"

namespace editor
{
	class input_manager : public inp::input_manager
	{
	public:
		input_manager()
			: inp::input_manager("editor", 0) 
		{
			set_active_layer("editor");
		}

		virtual ~input_manager() override {};
		virtual bool on_handle_event(const inp::input_event&) override;

		virtual void on_notify_listeners(float dt) override;

		// there is unblock input to next levels for one frame.
		// by default input manager is blocking 
		// keyboard_event, mouse_click_event with KEY_ACTION::DOWN and scroll_move_event
		void unblock_layer_once();
		void block_layer();
		void unblock_layer();

	private:
		bool is_block_keyaction_down = true;
		bool is_block_keyaction_down_once = true;
	};

	using InputManagerRef = std::shared_ptr<input_manager>;
}

namespace edt = editor;