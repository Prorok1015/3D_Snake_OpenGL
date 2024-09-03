#include "edt_editor_system.h"
#include <debug_ui_api.h>
#include <application.h>
#include <gs_game_system.h>
#include <res_resource_system.h>
#include <res_resource_texture.h>
#include <rnd_render_system.h>
#include <scn_primitives.h>
#include <camera/rnd_camera.h>
#include <ecs/ecs_common_system.h>
#include <scn_primitives.h>

#include <light/rnd_light_point.h>
#include <sky/rnd_cubemap.h>

#include <imgui.h>

editor::EditorSystem::EditorSystem()
{
	DBG_UI_REG_LAMBDA("EDITOR/GUI/TOOLBAR", [this] { return show_toolbar(); });
	DBG_UI_SET_ITEM_CHECKED("EDITOR/GUI/TOOLBAR", true);

	DBG_UI_REG_LAMBDA("EDITOR/TEST/ECS", [this] { return show_ecs_test(); });
	DBG_UI_SET_ITEM_CHECKED("EDITOR/TEST/ECS", false);

	DBG_UI_REG_LAMBDA("EDITOR/IMPL/SHOW_WEP", [this] { return show_web(); });
	DBG_UI_SET_ITEM_CHECKED("EDITOR/IMPL/SHOW_WEP", true);

	DBG_UI_REG_LAMBDA("EDITOR/SHOW_WEP", [this] { return true; });
	DBG_UI_SET_ITEM_CHECKED("EDITOR/SHOW_WEP", is_show_web);

	auto logo = res::get_system().require_resource<res::Picture>(res::Tag::make("icons/editor_engine_logo.png"));
	gs::get_system().get_window()->set_logo(logo);
	gs::get_system().get_window()->set_title("Snake Editor");

	glm::ivec4 viewport{ glm::zero<glm::ivec2>(), gs::get_system().get_window()->get_size() };

	camera = std::make_shared<rnd::camera>(glm::vec3(0), viewport);
	ecs::add_component(camera->ecs_entity, scn::is_render_component_flag{});
	camera_controller = std::make_shared<scn::mouse_camera_controller>(camera.get());
	camera_controller->enable_input_actions(gs::get_system().get_input_manager());

	second_camera = std::make_shared<rnd::camera>(glm::vec3(10, 10, 10), viewport);
	second_camera_controller = std::make_shared<scn::mouse_camera_controller>(second_camera.get());

	gs::get_system().get_window()->eventResizeWindow.subscribe([](wnd::window&, int w, int h)
	{
		for (auto& ent : ecs::filter<rnd::camera_component>())
		{
			auto* camera = ecs::get_component<rnd::camera_component>(ent);
			camera->viewport[2] = w;
			camera->viewport[3] = h;
		}
	});

	auto txt = rnd::get_system().get_texture_manager().generate_texture(res::Tag::make("__black"), { 1,1 }, 3, { 0, 0, 0 });

	scn::Model web = generate_web({ 50, 50 });
	web.meshes.back().material.texture_tag = res::Tag::make("__black");
	web.model = glm::scale(web.model, glm::vec3(20, 0, 20));

	editor_web = ecs::create_entity();
	ecs::add_component(editor_web, scn::model_comonent{ web.meshes });
	ecs::add_component(editor_web, scn::transform_component{ web.model });
	ecs::add_component(editor_web, scn::is_render_component_flag{});
	ecs::add_component(editor_web, rnd::render_mode_component{rnd::RENDER_MODE::LINE});

	light = ecs::create_entity();
	ecs::add_component(light, rnd::light_point{ .light_color = glm::vec3(1.0) });
	ecs::add_component(light, scn::transform_component{ .world = glm::translate(glm::mat4{1.0}, glm::vec3(50, 50, 50)) });
	scn::Model m = generate_sphere();
	ecs::add_component(light, scn::model_comonent{ m.meshes });
	ecs::add_component(light, scn::is_render_component_flag{});

	sky = ecs::create_entity();
	ecs::add_component(sky, rnd::cubemap_component{ rnd::get_system().get_texture_manager().require_cubemap_texture({
		res::Tag::make("skybox/right.jpg"),
		res::Tag::make("skybox/left.jpg"),
		res::Tag::make("skybox/bottom.jpg"),
		res::Tag::make("skybox/top.jpg"),
		res::Tag::make("skybox/back.jpg"),
		res::Tag::make("skybox/front.jpg"),
		}) 
	});
	ecs::add_component(sky, scn::is_render_component_flag{});

}


editor::EditorSystem::~EditorSystem()
{
	camera_controller->disable_input_actions(gs::get_system().get_input_manager());
	second_camera_controller->disable_input_actions(gs::get_system().get_input_manager());
}


bool editor::EditorSystem::show_toolbar()
{
	bool is_open = true;
	ImGui::SetNextWindowSize(ImVec2{ 200, 400 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Observer", &is_open))
	{
		auto& app = app::get_app_system();
		ImGui::Text("Common");
		ImGui::Separator();
		ImGui::ColorEdit4("Clear color", glm::value_ptr(clear_color));
		rnd::get_system().get_driver()->set_clear_color(clear_color);
		ImGui::Separator();
		ImGui::NewLine();

		ImGui::Text("Shader");
		ImGui::Separator();

		if (ImGui::Button("Reload Shaders")) {
			gs::get_system().reload_shaders();
		}
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("Light");
		ImGui::Separator();
		{
			for (auto& ent : ecs::filter<rnd::light_point, scn::transform_component>()) {

				ImGui::ColorEdit3("Light color", glm::value_ptr(ecs::get_component<rnd::light_point>(ent)->light_color));

				eng::transform3d ct{ ecs::get_component<scn::transform_component>(ent)->world };
				ImGui::Text("pitch: %.3f, yaw: %.3f, roll: %.3f", glm::degrees(ct.get_pitch()), glm::degrees(ct.get_yaw()), glm::degrees(ct.get_roll()));
				auto pos = ct.get_pos();
				ImGui::Text("x: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);
				glm::vec3 vec4f = ct.get_pos();
				ImGui::InputFloat3("light position", glm::value_ptr(vec4f));
				if (vec4f != ct.get_pos()) {
					ct.set_pos(vec4f);
					ecs::get_component<scn::transform_component>(ent)->world = ct.to_matrix();
				}
			}
		}

		ImGui::Separator();
		ImGui::NewLine();
		{
			ImGui::Text("Camera");
			ImGui::Separator();
			const char* items[] = { "Main", "Second"};
			static int item_current = 0;
			static int item_old = 0;

			ImGui::Combo("Choose camera", &item_current, items, IM_ARRAYSIZE(items));
			if (item_current != item_old) {
				if (item_current == 0) {
					ecs::add_component(camera->ecs_entity, scn::is_render_component_flag{});
					ecs::remove_component<scn::is_render_component_flag>(second_camera->ecs_entity);
					second_camera_controller->disable_input_actions(gs::get_system().get_input_manager());
					camera_controller->enable_input_actions(gs::get_system().get_input_manager());
				} else {
					ecs::remove_component<scn::is_render_component_flag>(camera->ecs_entity);
					ecs::add_component(second_camera->ecs_entity, scn::is_render_component_flag {});
					camera_controller->disable_input_actions(gs::get_system().get_input_manager());
					second_camera_controller->enable_input_actions(gs::get_system().get_input_manager());
				}
				item_old = item_current;
			}

			for (auto& ent : ecs::filter<rnd::camera_component, scn::is_render_component_flag>()) {
				eng::transform3d ct{ ecs::get_component<rnd::camera_component>(ent)->world };
				ImGui::Text("pitch: %.3f, yaw: %.3f, roll: %.3f", glm::degrees(ct.get_pitch()), glm::degrees(ct.get_yaw()), glm::degrees(ct.get_roll()));
				auto pos = ct.get_pos();
				ImGui::Text("x: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);
			}
		}

		ImGui::Text("Scene");
		ImGui::Separator();

		ImGui::InputText("Name", buf, 64);
		if (ImGui::Button("Add model")) {
			gs::get_system().load_model(buf);
		}
		static int cube_count_add = 1;
		static int cube_count_remove = 1;
		static int cube_count = 0;
		static float distance = camera->get_visible_distance();

		ImGui::SliderFloat("Object scale", &distance, -0.9f, 400.f);

		if (ImGui::Button("add cube")) {
			for (int i = 0; i < cube_count_add; ++i) {
				gs::get_system().add_cube_to_scene(20.f);
				cube_count++;
			}
		}		
		ImGui::SameLine();
		ImGui::SliderInt("count add", &cube_count_add, 1, 100);
		ImGui::NewLine();

		if (ImGui::Button("remove cube")) {
			for (int i = 0; i < cube_count_remove; ++i) {
				gs::get_system().remove_cube();
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

		ImGui::Combo("render mode", &item_current, items, IM_ARRAYSIZE(items));
		if (item_current != item_old) {
			static std::unordered_map<std::string, rnd::RENDER_MODE> mmap{ 
				{items[0], rnd::RENDER_MODE::TRIANGLE}, 
				{items[1], rnd::RENDER_MODE::TRIANGLE_STRIP},
				{items[2], rnd::RENDER_MODE::LINE_LOOP},
				{items[3], rnd::RENDER_MODE::LINE_STRIP},
				{items[4], rnd::RENDER_MODE::LINE},
				{items[5], rnd::RENDER_MODE::POINT},
			};
			rnd::get_system().set_render_mode(mmap[items[item_current]]);
			item_old = item_current;
		}

		eng::transform3d ct;
		ImGui::Text("pitch: %.3f, yaw: %.3f, roll: %.3f", glm::degrees(ct.get_pitch()), glm::degrees(ct.get_yaw()), glm::degrees(ct.get_roll()));
		auto pos = ct.get_pos();
		ImGui::Text("x: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);

		ImGui::End();
	}

	return is_open;
}

bool editor::EditorSystem::show_web()
{
	const bool cur_is_show = DBG_UI_IS_ITEM_CHECKED("EDITOR/SHOW_WEP");
	if (!cur_is_show && cur_is_show != is_show_web) {
		ecs::remove_component<scn::is_render_component_flag>(editor_web);
		ecs::remove_component<scn::is_render_component_flag>(sky);
	}
	else if (cur_is_show && cur_is_show != is_show_web){
		ecs::add_component(editor_web, scn::is_render_component_flag{});
		ecs::add_component(sky, scn::is_render_component_flag{});
	}

	is_show_web = cur_is_show;
	return true;
}

bool editor::EditorSystem::show_ecs_test()
{
	init_ecs_test();

	bool is_open = true;
	ImGui::SetNextWindowSize(ImVec2{ 400, 300 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("ECS Test", &is_open))
	{
		for (auto ent : ecs::filter<eng::transform3d, glm::vec2>()) {
			eng::transform3d* trn = ecs::get_component<eng::transform3d>(ent);
			glm::vec2* v = ecs::get_component<glm::vec2>(ent);
			if (trn == nullptr) {
				ImGui::Text("entity: %d, null", ent.index);
				continue;
			}
			ImGui::Text("entity: %d, pitch: %.3f, yaw: %.3f, roll: %.3f", ent.index, trn->get_pitch(), trn->get_yaw(), trn->get_roll());
			auto pos = trn->get_pos();
			ImGui::Text("\t\tx: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);
			ImGui::Text("\t\tx: %.3f, y: %.3f", v->x, v->y);
		}

		ImGui::Separator();
		ImGui::NewLine();

		for (auto ent : ecs::filter<eng::transform3d>()) {
			eng::transform3d* trn = ecs::get_component<eng::transform3d>(ent);
			if (trn == nullptr) {
				ImGui::Text("entity: %d, null", ent.index);
				continue;
			}
			ImGui::Text("entity: %d, pitch: %.3f, yaw: %.3f, roll: %.3f", ent.index, trn->get_pitch(), trn->get_yaw(), trn->get_roll());
			auto pos = trn->get_pos();
			ImGui::Text("\t\tx: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);
		}

		ImGui::End();
	}

	return is_open;
}

bool editor::EditorSystem::init_ecs_test()
{
	if (is_inited_ecs_test) {
		return true;
	}

	is_inited_ecs_test = true;
	ecs::entity first = ecs::create_entity();
	eng::transform3d* trn = ecs::add_component(first, eng::transform3d{});
	glm::vec2* tmp = ecs::add_component(first, glm::vec2{ 9, 9 });
	trn->set_pos(glm::vec3{ 6, 6, 6 });
	ecs::entity second = ecs::create_entity();
	eng::transform3d* trn2 = ecs::add_component(second, eng::transform3d{ glm::translate(glm::mat4{1.0}, glm::vec3(3)) });
	trn2->set_yaw(45.f);

	return true;
}
