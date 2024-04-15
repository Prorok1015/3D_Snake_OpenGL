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
	auto logo = res::get_system().require_resource<res::Image>(res::Tag::make("icons/editor_engine_logo.png"));
	gm::get_system().get_window()->set_logo(logo, nullptr);
}


bool editor::EditorSystem::show_toolbar()
{
	bool is_open = true;
	ImGui::SetNextWindowSize(ImVec2{ 200, 400 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Observer", &is_open, ImGuiWindowFlags_NoTitleBar))
	{
		auto& app = app::get_app_system();
		ImGui::Text("Common");
		ImGui::Separator();
		ImGui::ColorEdit4("Clear color", glm::value_ptr(app.clear_color_));
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Shader");
		ImGui::Separator();

		ImGui::Checkbox("Show normals", &gm::get_system().is_show_normal);

		if (ImGui::Button("Reload Shaders")) {
			gm::get_system().reload_shaders();
		}

		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Scene");
		ImGui::Separator();

		ImGui::InputText("Name", buf, 64);
		if (ImGui::Button("Reload")) {
			gm::get_system().load_model(buf);
		}
		static int cube_count_add = 1;
		static int cube_count_remove = 1;
		static int cube_count = 0;
		ImGui::SliderFloat("Object scale", &gm::get_system().cube_scale, 0.1f, 10.f);
		if (ImGui::Button("add cube")) {
			for (int i = 0; i < cube_count_add; ++i) {
				gm::get_system().add_cube_to_scene(10.f);
				cube_count++;
			}
		}		
		ImGui::SameLine();
		ImGui::SliderInt("count add", &cube_count_add, 1, 100);
		ImGui::NewLine();

		if (ImGui::Button("remove cube")) {
			for (int i = 0; i < cube_count_remove; ++i) {
				gm::get_system().remove_cube();
				if (cube_count > 0)
					cube_count--;
			}
		}
		ImGui::SameLine();
		ImGui::SliderInt("count remove", &cube_count_remove, 1, 100);
		ImGui::NewLine();

		ImGui::Text("cubes count: %d", cube_count);

		ImGui::End();
	}

	return is_open;
}