#include "debug_ui_system.h"
#include "../game_system/game_system.h"

debug_ui::DebugUiSystem* p_dbgui_system = nullptr;

debug_ui::DebugUiSystem& debug_ui::get_system()
{
	ASSERT_MSG(p_dbgui_system, "DebugUi system is nullptr!");
	return *p_dbgui_system;
}

debug_ui::DebugUiSystem::DebugUiSystem()
{
	auto& game = gm::get_system();
	inp::InputSystem& inpSystem = inp::get_system();

	inpSystem.sub_keyboard_by_tag<DebugUiSystem>([this](auto code, auto action) { switch_input(code, action); });

	auto win = game.get_window();
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(win->get_id(), enable_input);
	ImGui_ImplOpenGL3_Init("#version 130");

	registrate_menu("DEMO/SHOW DEMO WINDOW", [this] { return show_demo(); });
	registrate_menu("MENU/STATISTIC WINDOW", [this] {  return show_stats(); });
}

debug_ui::DebugUiSystem::~DebugUiSystem()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void debug_ui::DebugUiSystem::render()
{
	// Rendering
	ImGui::Render();
}


void debug_ui::DebugUiSystem::begin_frame()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void debug_ui::DebugUiSystem::end_frame()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void debug_ui::DebugUiSystem::capture()
{
	if (is_hiden()) {
		return;
	}

	menu.process();
}

void debug_ui::DebugUiSystem::switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action)
{
	if (code == inp::KEYBOARD_BUTTONS::F5 && action == inp::KEY_ACTION::UP)
	{
		auto& game = gm::get_system();
		game.set_enable_input(menu.is_show_main_menu());
		menu.set_show_main_menu(!menu.is_show_main_menu());
	}
}

void debug_ui::DebugUiSystem::set_enable_input(bool enable)
{
	enable_input = enable;
	auto& game = gm::get_system();

	if (enable_input) {
		ImGui_ImplGlfw_InstallCallbacks(game.get_window()->get_id());
		game.get_window()->set_cursor_mode(CursorMode::Normal);
	} else {
		ImGui_ImplGlfw_RestoreCallbacks(game.get_window()->get_id());
	}
}

void debug_ui::DebugUiSystem::registrate_menu(std::string_view path, DebugUiMainMenu::UI_CALLBACK callback)
{
	menu.registrate(path, callback);
}

void debug_ui::DebugUiSystem::unregistrate_menu(std::string_view path)
{
	menu.unregistrate(path);
}

void debug_ui::DebugUiSystem::register_implicit(const std::string_view id, DebugUiMainMenu::UI_CALLBACK callback)
{
	menu.register_implicit(id, callback);
}

void debug_ui::DebugUiSystem::unregister_implicit(const std::string_view id)
{
	menu.unregister_implicit(id);
}

void debug_ui::DebugUiSystem::set_menu_checked(const std::string_view path, bool checked)
{
	menu.set_menu_checked(path, checked);
}

bool debug_ui::DebugUiSystem::is_item_checked(const std::string_view path) const
{
	return menu.is_item_checked(path);
}

void debug_ui::DebugUiSystem::set_check_callback(const std::string_view path, DebugUiMainMenu::UI_SWITCH_CALLBACK callback)
{
	menu.set_check_callback(path, callback);
}

bool debug_ui::DebugUiSystem::show_stats()
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

bool debug_ui::DebugUiSystem::show_demo()
{
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
	return show_demo_window;
}
