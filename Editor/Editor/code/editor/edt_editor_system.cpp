#include "edt_editor_system.h"
#include <engine/debug_ui/debug_ui_api.h>
#include <engine/application/application.h>
#include <engine/game_system/game_system.h>
#include <engine/resource/res_resource_system.h>
#include <engine/resource/res_resource_texture.h>

editor::EditorSystem::EditorSystem()
{
	DBG_UI_REG_LAMBDA("GAME/UI/TOOLBAR", [this] { return show_toolbar(); });
	DBG_UI_SET_ITEM_CHECKED("GAME/UI/TOOLBAR", true);
	gm::get_system().load_model(buf);
	auto logo = res::get_system().require_resource<res::Texture>(res::Tag::make("icons/editor_engine_logo.png"));
	gm::get_system().get_window()->set_logo(logo, nullptr);
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
		ImGui::Separator();
		ImGui::InputText("Model name", buf, 64);
		if (ImGui::Button("Load model")) {
			gm::get_system().load_model(buf);
		}

		if (ImGui::SliderFloat("Cude Scale", &gm::get_system().cube_scale, 1.f, 10.f))
		{

		}

		if (ImGui::Button("Reload Shaders")) {
			gm::get_system().reload_shaders();
		}

		ImGui::Checkbox("Show normals", &gm::get_system().is_show_normal);

		ImGui::End();
	}

	return is_open;
}