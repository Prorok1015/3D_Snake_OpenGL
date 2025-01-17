#include "gui_system.h"
#include "engine_assert.h"
#include "rnd_render_system.h"
#include "wnd_window_system.h"

gui::gui_system* p_gui_system = nullptr;

gui::gui_system& gui::get_system()
{
	ASSERT_MSG(p_gui_system, "DebugUi system is nullptr!");
	return *p_gui_system;
}

gui::gui_system::gui_system()
{
	registrate_menu("Debug/Imgui demo window", [this] { return show_demo(); });
	registrate_menu("Debug/Statistic", [this] {  return show_stats(); });
	set_menu_checked("Degub/Statistic", true);

	renderer = std::make_shared<gui::renderer>(wnd::get_system().get_gui_backend());
	renderer->render_event += [this] { render_menues(); };
	rnd::get_system().activate_renderer(renderer);
}

gui::gui_system::~gui_system()
{
	rnd::get_system().deactivate_renderer(renderer);
}

void gui::gui_system::render_menues()
{
	if (is_hiden()) {
		return;
	}

	menu.process();
}

void gui::gui_system::set_show_title_bar(bool show)
{
	menu.set_show_main_menu(show);
}

//void gui::gui_system::switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action)
//{
//	if (code == inp::KEYBOARD_BUTTONS::F5 && action == inp::KEY_ACTION::UP)
//	{
//		gs::get_system().set_enable_input(menu.get_is_show_main_menu());
//		menu.set_show_main_menu(!menu.get_is_show_main_menu());
//		ImGui::SetWindowFocus();
//	}
//}

void gui::gui_system::set_is_input_enabled(bool enable)
{
	if (is_input_enabled == enable) {
		return;
	}

	is_input_enabled = enable;
}

void gui::gui_system::registrate_menu(std::string_view path, gui::menu_layout_manager::UI_CALLBACK callback)
{
	menu.registrate(path, callback);
}

void gui::gui_system::unregistrate_menu(std::string_view path)
{
	menu.unregistrate(path);
}

void gui::gui_system::register_implicit(const std::string_view id, gui::menu_layout_manager::UI_CALLBACK callback)
{
	menu.register_implicit(id, callback);
}

void gui::gui_system::unregister_implicit(const std::string_view id)
{
	menu.unregister_implicit(id);
}

void gui::gui_system::set_menu_checked(const std::string_view path, bool checked)
{
	menu.set_menu_checked(path, checked);
}

bool gui::gui_system::is_item_checked(const std::string_view path) const
{
	return menu.get_is_item_checked(path);
}

void gui::gui_system::set_check_callback(const std::string_view path, gui::menu_layout_manager::UI_SWITCH_CALLBACK callback)
{
	menu.set_check_callback(path, callback);
}

bool gui::gui_system::show_stats()
{
	bool is_open = true;
	//ImGui::SetNextWindowSize(ImVec2{ 400, 60 }, ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowPos(ImVec2{ 0, 20 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Common stats", &is_open))
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	}
	ImGui::End();
	return is_open;
}

bool gui::gui_system::show_demo()
{
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
	return show_demo_window;
}
