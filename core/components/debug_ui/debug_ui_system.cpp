#include "debug_ui_system.h"
#include <wnd_window_system.h>
#include <inp_input_system.h>
#include <rnd_render_system.h>
#include <gs_game_system.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

dbg_ui::DebugUiSystem* p_dbgui_system = nullptr;

dbg_ui::DebugUiSystem& dbg_ui::get_system()
{
	ASSERT_MSG(p_dbgui_system, "DebugUi system is nullptr!");
	return *p_dbgui_system;
}

dbg_ui::DebugUiSystem::DebugUiSystem()
{ 
	inp::InputSystem& inpSystem = inp::get_system();

	inpSystem.sub_keyboard_by_tag<DebugUiSystem>([this](auto code, auto action) { switch_input(code, action); });

	window = wnd::get_system().get_active_window();
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window->get_id(), is_input_enabled);
	ImGui_ImplOpenGL3_Init("#version 130");

	registrate_menu("DEMO/SHOW DEMO WINDOW", [this] { return show_demo(); });
	registrate_menu("MENU/STATISTIC WINDOW", [this] {  return show_stats(); });

	renderer = std::make_shared<dbg_ui::renderer>();
	renderer->render_event += [this] { render_menues(); };
	rnd::get_system().registrate_renderer(renderer);
}

dbg_ui::DebugUiSystem::~DebugUiSystem()
{
	rnd::get_system().unregistrate_renderer(renderer);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void dbg_ui::DebugUiSystem::render_menues()
{
	if (is_hiden()) {
		return;
	}

	menu.process();
}

void dbg_ui::DebugUiSystem::switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action)
{
	if (code == inp::KEYBOARD_BUTTONS::F5 && action == inp::KEY_ACTION::UP)
	{ 
		// TODO move switching to other object who known about debug_ui and gs
		gs::get_system().set_enable_input(menu.is_show_main_menu());
		menu.set_show_main_menu(!menu.is_show_main_menu());
	}
}

void dbg_ui::DebugUiSystem::set_is_input_enabled(bool enable)
{
	is_input_enabled = enable;

	if (is_input_enabled) {
		ImGui_ImplGlfw_InstallCallbacks(window->get_id());
		window->set_cursor_mode(CursorMode::Normal);
	} else {
		ImGui_ImplGlfw_RestoreCallbacks(window->get_id());
	}
}

void dbg_ui::DebugUiSystem::registrate_menu(std::string_view path, dbg_ui::DebugUiMainMenu::UI_CALLBACK callback)
{
	menu.registrate(path, callback);
}

void dbg_ui::DebugUiSystem::unregistrate_menu(std::string_view path)
{
	menu.unregistrate(path);
}

void dbg_ui::DebugUiSystem::register_implicit(const std::string_view id, dbg_ui::DebugUiMainMenu::UI_CALLBACK callback)
{
	menu.register_implicit(id, callback);
}

void dbg_ui::DebugUiSystem::unregister_implicit(const std::string_view id)
{
	menu.unregister_implicit(id);
}

void dbg_ui::DebugUiSystem::set_menu_checked(const std::string_view path, bool checked)
{
	menu.set_menu_checked(path, checked);
}

bool dbg_ui::DebugUiSystem::is_item_checked(const std::string_view path) const
{
	return menu.is_item_checked(path);
}

void dbg_ui::DebugUiSystem::set_check_callback(const std::string_view path, dbg_ui::DebugUiMainMenu::UI_SWITCH_CALLBACK callback)
{
	menu.set_check_callback(path, callback);
}

bool dbg_ui::DebugUiSystem::show_stats()
{
	bool is_open = true;
	if (ImGui::Begin("Hello, world!", &is_open))                      
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}
	return is_open;
}

bool dbg_ui::DebugUiSystem::show_demo()
{
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
	return show_demo_window;
}
