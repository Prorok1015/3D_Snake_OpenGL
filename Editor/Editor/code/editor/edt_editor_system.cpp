#include "edt_editor_system.h"
#include <engine/debug_ui/debug_ui_api.h>
#include <engine/application/application.h>
#include <engine/game_system/game_system.h>
#include <engine/resource/res_resource_system.h>
#include <engine/resource/res_resource_texture.h>
#include <engine/render/rnd_render_system.h>
#include <engine/scene/primitives.h>

editor::EditorSystem::EditorSystem()
{
	DBG_UI_REG_LAMBDA("GAME/UI/TOOLBAR", [this] { return show_toolbar(); });
	DBG_UI_SET_ITEM_CHECKED("GAME/UI/TOOLBAR", true);

	auto logo = res::get_system().require_resource<res::Image>(res::Tag::make("icons/editor_engine_logo.png"));
	gm::get_system().get_window()->set_logo(logo, nullptr);

	g_Scene = generate_network({ 50, 50 });

	auto txt = rnd::get_system().get_txr_manager().generate_texture(res::Tag::make("__black"), { 1,1 }, 3, { 0, 0, 0 });

	g_Scene.meshes.back().material.texture = res::Tag::make("__black");
	g_Scene.model = glm::scale(g_Scene.model, glm::vec3(20, 0, 20));
	rnd::get_system().set_line_size(2);
}


editor::EditorSystem::~EditorSystem()
{

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
		ImGui::ColorEdit4("Clear color", glm::value_ptr(clear_color_));
		rnd::get_system().clear_color(clear_color_);
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

		ImGui::Checkbox("Is Draw Scene Net", &is_draw_scene_net);

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

		const char* items[] = { "TRIANGLE", "TRIANGLE_STRIP", "LINE_LOOP", "LINE_STRIP", "LINE", "POINT"};
		static int item_current = 0;
		static int item_old = 0;

		ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
		if (item_current != item_old) {
			static std::unordered_map<std::string, rnd::RENDER_MODE> mmap{ 
				{items[0], rnd::RENDER_MODE::TRIANGLE}, 
				{items[1], rnd::RENDER_MODE::TRIANGLE_STRIP},
				{items[2], rnd::RENDER_MODE::LINE_LOOP},
				{items[3], rnd::RENDER_MODE::LINE_STRIP},
				{items[4], rnd::RENDER_MODE::LINE},
				{items[5], rnd::RENDER_MODE::POINT},
			};
			rnd::get_system().render_mode(mmap[items[item_current]]);
			item_old = item_current;
		}

		scn::Transform ct = game::get_system().get_camera_transform();
		ImGui::Text("pitch: %.3f, yaw: %.3f, roll: %.3f", glm::degrees(ct.get_pitch()), glm::degrees(ct.get_yaw()), glm::degrees(ct.get_roll()));
		auto pos = ct.get_pos();
		ImGui::Text("x: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);

		ImGui::End();
	}

	return is_open;
}

void editor::EditorSystem::pre_render()
{
	if (is_draw_scene_net) {
		auto tmp = rnd::get_system().render_mode();
		rnd::get_system().render_mode(rnd::RENDER_MODE::LINE);


		rnd::get_system().get_renderer().draw(g_Scene);

		rnd::get_system().render_mode(tmp);
	}
}
