#pragma once
#include "../common/common.h"

namespace debug_ui
{
	class DebugUiSystem 
	{
	public:
		DebugUiSystem();
		~DebugUiSystem();

		DebugUiSystem(const DebugUiSystem&) = default;
		DebugUiSystem(DebugUiSystem&&) = default;
		DebugUiSystem& operator= (const DebugUiSystem&) = default;
		DebugUiSystem& operator= (DebugUiSystem&&) = default;

		void render();
		void begin_frame();
		void end_frame();
		void capture();

	private:
		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	};
}

namespace dbg_ui = debug_ui;