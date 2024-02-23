#include "edt_editor_system.h"
#include "../debug_ui/debug_ui_api.h"
#include "../application/application.h"

editor::EditorSystem::EditorSystem()
{
	DBG_UI_REG_LAMBDA("GAME/UI/TOOLBAR", [this] { return show_toolbar(); });
	DBG_UI_SET_ITEM_CHECKED("GAME/UI/TOOLBAR", true);
}


bool editor::EditorSystem::show_toolbar()
{
	bool is_open = true;
	ImGui::SetNextWindowSize(ImVec2{ 200, 400 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Observer", &is_open, ImGuiWindowFlags_NoTitleBar))
	{
		auto& app = app::get_app_system();
		glm::vec4 ref_color = app.get_clear_color();
		if (ImGui::ColorEdit4("Clear color", (float*)&ref_color))
		{
			app.set_clear_color(ref_color);
		}
		ImGui::End();
	}

	return is_open;
}