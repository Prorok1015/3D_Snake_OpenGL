#pragma once
#include "../common/common.h"

namespace debug_ui
{
	class DebugUiMainMenu
	{
	public:
		using UI_CALLBACK = std::function<bool()>;
		using UI_SWITCH_CALLBACK = std::function<void(bool)>;
		struct MenuTreeItem 
		{
			MenuTreeItem() = default;
			MenuTreeItem(const std::string_view title)
				: title(title){};

			std::string get_path() const;
			MenuTreeItem* find(const std::string_view title) const;
			void add(MenuTreeItem* item);
			void remove(MenuTreeItem* item);
			void notify_check_callback(bool enable);

		public:
			bool checked = false;
			MenuTreeItem* parent = nullptr;
			UI_CALLBACK uiCallback;
			UI_SWITCH_CALLBACK checkCallback;
			std::string title;
			std::vector<MenuTreeItem*> childs;
		};

		void registrate(std::string_view path, std::function<bool()> callback);
		void unregistrate(std::string_view path);

		void register_implicit(const std::string_view id, UI_CALLBACK callback);
		void unregister_implicit(const std::string_view id);

		void set_menu_checked(const std::string_view path, bool checked);
		bool is_item_checked(const std::string_view path) const;
		void set_check_callback(const std::string_view path, UI_SWITCH_CALLBACK callback);

		void uncheck_all();

		void process();

		void set_show_main_menu(bool show) { is_show_main_menu_ = show; }
		bool is_show_main_menu() const { return is_show_main_menu_; };

	private:
		void add_menu(std::string_view path, UI_CALLBACK cb);
		void process_menu_item(MenuTreeItem* menuItem);
		void delete_menu(MenuTreeItem*);
		void set_menu_checked(MenuTreeItem* menuItem, bool checked);
		MenuTreeItem* find_menu(const std::string_view path);
		std::vector<std::string> split(std::string_view, std::string_view);

	private:
		bool is_show_main_menu_ = false;
		MenuTreeItem rootMenuItem;

		std::vector<MenuTreeItem*> checkedMenuItems;
		std::vector<std::pair<std::string, UI_CALLBACK>> implicitCallbacks;
	};

}

namespace dbg_ui = debug_ui;