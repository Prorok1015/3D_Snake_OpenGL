#pragma once
#include "../common/common.h"
#include "../common/enums.h"
#include "../input/inp_key_enums.hpp"

namespace debug_ui
{
	class DebugUiSystem 
	{
	public:
		DebugUiSystem();
		~DebugUiSystem();

		DebugUiSystem(DebugUiSystem&&) = default;
		DebugUiSystem& operator= (DebugUiSystem&&) = default;

		DebugUiSystem(const DebugUiSystem&) = delete;
		DebugUiSystem& operator= (const DebugUiSystem&) = delete;

		void render();
		void begin_frame();
		void end_frame();
		void capture();

		bool is_hiden() const { return !enable_input; }

		void switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action);
		void set_enable_input(bool enable);

	private:
		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		bool enable_input = false;
	};

	DebugUiSystem& get_system();
}

namespace dbg_ui = debug_ui;