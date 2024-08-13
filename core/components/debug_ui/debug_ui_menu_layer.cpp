#include "debug_ui_menu_layer.h"
#include <imgui.h>
#include <ranges>
#include <engine_assert.h>

namespace debug_ui
{

void DebugUiMainMenu::process()
{
	if (is_show_main_menu()) {
		if (ImGui::BeginMainMenuBar()) {
			for (auto& child : rootMenuItem.childs) {
				process_menu_item(child);
			}
			ImGui::EndMainMenuBar();
		}
	}

	std::vector<MenuTreeItem*> del;
	for (int i = 0; i < checkedMenuItems.size(); ++i) {
		MenuTreeItem* item = checkedMenuItems[i];
		item->checked = item->uiCallback();
		if (!item->checked) {
			del.push_back(item);
			item->notify_check_callback(false);
		}
	}

	for (auto& it : del)
	{
		checkedMenuItems.erase(std::remove(checkedMenuItems.begin(), checkedMenuItems.end(), it));
	}

	std::vector<std::string> del_imp;
	for (int i = 0; i < implicitCallbacks.size(); ++i) {
		if (!implicitCallbacks[i].second()) {
			del_imp.push_back(implicitCallbacks[i].first);
		}
	}

	for (auto& imp : del_imp) {
		auto it = std::remove_if(implicitCallbacks.begin(), implicitCallbacks.end(), [imp](auto& i) {return imp == i.first; });
		implicitCallbacks.erase(it);
	}
}

void DebugUiMainMenu::registrate(std::string_view path, std::function<bool()> callback)
{
	add_menu(path, callback);

}

void DebugUiMainMenu::unregistrate(std::string_view path)
{
	std::vector<std::string> tokens = split(path, "/");
	MenuTreeItem* menuItem = &rootMenuItem;

	for (auto& token : tokens) {
		MenuTreeItem* curMenuItem = menuItem->find(token);
		if (!curMenuItem) {
			return;
		}
		menuItem = curMenuItem;
	}

	while (true) {
		MenuTreeItem* parent = menuItem->parent;
		if (parent->childs.size() != 1 || parent == &rootMenuItem) {
			parent->remove(menuItem);
			break;
		}
		menuItem = parent;
	}
	delete_menu(menuItem);
}

void DebugUiMainMenu::register_implicit(const std::string_view id, UI_CALLBACK callback)
{
	auto idx = std::ranges::find_if(implicitCallbacks, [id](auto& i) { return i.first == id; });
	if (idx == implicitCallbacks.end()) {
		implicitCallbacks.push_back(std::pair<std::string, UI_CALLBACK>(id, std::move(callback)));
	}
}

void DebugUiMainMenu::unregister_implicit(const std::string_view id)
{
	implicitCallbacks.erase(std::remove_if(implicitCallbacks.begin(), implicitCallbacks.end(), [id](auto& i) { return i.first == id; }));
}

std::vector<std::string> DebugUiMainMenu::split(std::string_view path, std::string_view delim)
{
	std::vector<std::string> tokens;
	for (const auto& word : std::views::split(path, delim)) {
		tokens.emplace_back(std::string_view{ word.data(), word.size() });
	}

	return tokens;
}


void DebugUiMainMenu::add_menu(std::string_view path, UI_CALLBACK cb)
{
	std::vector<std::string> tokens = split(path, "/");

	MenuTreeItem* menuItem = &rootMenuItem;

	for (auto& token : tokens) {
		MenuTreeItem* curMenuItem = menuItem->find(token);
		if (!curMenuItem) {
			curMenuItem = new MenuTreeItem(token);
			menuItem->add(curMenuItem);
		}
		menuItem = curMenuItem;
	}

	ASSERT_MSG(menuItem->uiCallback == nullptr, "Debug menu item already exist"/*, path.CStr()*/);
	menuItem->uiCallback = std::move(cb);
}

void DebugUiMainMenu::process_menu_item(MenuTreeItem* menuItem)
{
	if (!menuItem->uiCallback) {
		if (ImGui::BeginMenu(menuItem->title.c_str())) {
			for (auto& child : menuItem->childs) {
				process_menu_item(child);
			}
			ImGui::EndMenu();
		}
	}
	else {
		if (ImGui::MenuItem(menuItem->title.c_str(), nullptr, &menuItem->checked)) {
			if (menuItem->checked) {
				checkedMenuItems.push_back(menuItem);
				menuItem->notify_check_callback(true);
			}
			else {
				auto id = std::remove(checkedMenuItems.begin(), checkedMenuItems.end(), menuItem);
				if (id != checkedMenuItems.end()) {
					checkedMenuItems.erase(id);
					menuItem->notify_check_callback(false);
				}
			}
		}
	}
}

DebugUiMainMenu::MenuTreeItem* DebugUiMainMenu::find_menu(const std::string_view path)
{
	std::vector<std::string> tokens = split(path, "/");

	MenuTreeItem* menuItem = &rootMenuItem;

	for (auto& token : tokens) {
		MenuTreeItem* curMenuItem = menuItem->find(token);
		if (!curMenuItem) {
			return nullptr;
		}
		menuItem = curMenuItem;
	}

	if (menuItem == &rootMenuItem) {
		return nullptr;
	}

	return menuItem;
}

void DebugUiMainMenu::set_menu_checked(MenuTreeItem* menuItem, bool checked)
{
	if (!menuItem->childs.empty()) {
		menuItem->checked = false;
		return;
	}

	menuItem->checked = checked;
	if (menuItem->checked) {
		checkedMenuItems.push_back(menuItem);
		menuItem->notify_check_callback(true);
	}
	else {
		auto id = std::remove(checkedMenuItems.begin(), checkedMenuItems.end(), menuItem);
		if (id != checkedMenuItems.end()) {
			checkedMenuItems.erase(id);
			menuItem->notify_check_callback(false);
		}
	}
}

void DebugUiMainMenu::set_menu_checked(const std::string_view path, bool checked)
{
	MenuTreeItem* menuItem = find_menu(path);
	if (!menuItem) {
		return;
	}

	set_menu_checked(menuItem, checked);
}

bool DebugUiMainMenu::is_item_checked(const std::string_view path) const
{
	return std::ranges::find_if(checkedMenuItems, [path](auto i) { return i->get_path() == path; }) != checkedMenuItems.end();
}

void DebugUiMainMenu::set_check_callback(const std::string_view path, UI_SWITCH_CALLBACK callback)
{
	MenuTreeItem* menuItem = find_menu(path);
	if (!menuItem) {
		return;
	}

	menuItem->checkCallback = std::move(callback);
}

void DebugUiMainMenu::uncheck_all()
{
	for (MenuTreeItem* item : checkedMenuItems) {
		item->checked = false;
	}
	checkedMenuItems.clear();
}

void DebugUiMainMenu::delete_menu(MenuTreeItem* menuItem)
{
	for (MenuTreeItem* child : menuItem->childs) {
		delete_menu(child);
	}

	set_menu_checked(menuItem, false);
	delete menuItem;
}

DebugUiMainMenu::MenuTreeItem* DebugUiMainMenu::MenuTreeItem::find(const std::string_view title) const
{
	for (const auto& child : childs) {
		if (child->title == title) {
			return child;
		}
	}
	return nullptr;
}


void DebugUiMainMenu::MenuTreeItem::add(MenuTreeItem* item)
{
	childs.push_back(item);
	item->parent = this;
}


void DebugUiMainMenu::MenuTreeItem::remove(MenuTreeItem* item)
{
	childs.erase(std::remove(childs.begin(), childs.end(), item));
}


std::string DebugUiMainMenu::MenuTreeItem::get_path() const
{
	std::string fullName;
	if (parent && parent->parent) {
		fullName = parent->get_path() + "/" + title;
		return fullName;
	}

	return title;
}


void DebugUiMainMenu::MenuTreeItem::notify_check_callback(bool enable)
{
	if (checkCallback) {
		checkCallback(enable);
	}
}

}
