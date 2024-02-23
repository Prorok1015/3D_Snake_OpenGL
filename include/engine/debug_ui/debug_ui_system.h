#pragma once
#include "../common/common.h"
#include "../common/enums.h"
#include "../input/inp_key_enums.hpp"
#include "debug_ui_menu_layer.h"

namespace debug_ui
{
	class DebugUiSystem 
	{
	public:
		struct Menu
		{
			std::string name_;
			std::vector<Menu> items_;
		};

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

		void set_show_ui(bool show) { is_show = show; }
		bool is_hiden() const { return !is_show; }

		void switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action);
		void set_enable_input(bool enable);

		void registrate_menu(std::string_view path, DebugUiMainMenu::UI_CALLBACK callback);
		void unregistrate_menu(std::string_view path);

		void register_implicit(const std::string_view id, DebugUiMainMenu::UI_CALLBACK callback);
		void unregister_implicit(const std::string_view id);

		void set_menu_checked(const std::string_view path, bool checked);
		bool is_item_checked(const std::string_view path) const;
		void set_check_callback(const std::string_view path, DebugUiMainMenu::UI_SWITCH_CALLBACK callback);

		bool show_stats();
		bool show_demo();
	private:
		bool is_show = true;
		bool enable_input = true;

		DebugUiMainMenu menu;
	};

	DebugUiSystem& get_system();
}

namespace dbg_ui = debug_ui;