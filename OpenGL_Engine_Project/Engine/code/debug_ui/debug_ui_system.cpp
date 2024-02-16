#include "debug_ui_system.h"
#include "../game_system/game_system.h"
#include "../common/enums.h"

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

	inpSystem.sub_keyboard_by_tag<DebugUiSystem>(
		[this](inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action) 
		{ 
			auto& game = gm::get_system();
			game.switch_input(code, action); 
			if (code == inp::KEYBOARD_BUTTONS::F5)
			{
				if (action == inp::KEY_ACTION::UP) {
					enable_input = !enable_input;
					if (enable_input) {
						ImGui_ImplGlfw_InstallCallbacks(game.get_window()->id_);
						game.get_window()->set_cursor_mode(CursorMode::Normal);
					} else {
						ImGui_ImplGlfw_RestoreCallbacks(game.get_window()->id_);
					}
				}
			}
		});

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
	ImGui_ImplGlfw_InitForOpenGL(win->id_, enable_input);
	ImGui_ImplOpenGL3_Init("#version 130");
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
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}
