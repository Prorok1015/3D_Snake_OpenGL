#pragma once
#include <common.h>
#include <enums.h>
#include <inp_key_enums.hpp>
#include <wnd_window.h>
#include "debug_ui_menu_layer.h"
#include "debug_ui_renderer.h"

namespace dbg_ui
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

		void render_menues();

		void set_show_ui(bool show) { is_show = show; }
		bool is_hiden() const { return !is_show; }

		void switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action);
		void set_is_input_enabled(bool enable);

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
		bool is_input_enabled = true;
		std::shared_ptr<wnd::window> window;
		std::shared_ptr<renderer> renderer;

		DebugUiMainMenu menu;
	};

	DebugUiSystem& get_system();
}
 