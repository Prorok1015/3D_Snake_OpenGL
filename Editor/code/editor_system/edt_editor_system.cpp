#include "edt_editor_system.h"
#include "gui_api.hpp"
#include "application.h"
#include "gs_game_system.h"
#include "res_system.h"
#include "res_picture.h"
#include "rnd_render_system.h"
#include "scn_primitives.h"
#include "scn_camera_component.hpp"
#include "scn_camera_controller_component.hpp"
#include "ecs_common_system.h"
#include "scn_primitives.h"
#include "eng_transform_3d.hpp"
#include "wnd_window_system.h"

#include <imgui.h>

editor::EditorSystem::EditorSystem()
{
	GUI_REG_LAMBDA("Window/Toolbar", [this] { return show_toolbar(); });
	GUI_SET_ITEM_CHECKED("Window/Toolbar", true);

	GUI_REG_LAMBDA("Window/Scene", [this] { return show_scene(); });
	GUI_SET_ITEM_CHECKED("Window/Scene", true);

	GUI_REG_LAMBDA("Editor/Test ECS window", [this] { return show_ecs_test(); });
	GUI_SET_ITEM_CHECKED("Editor/Test ECS window", false);

	//TODO: move to app callbacks
	GUI_REG_LAMBDA_IMPLICIT("EDITOR/IMPL/SHOW_WEP", [this] { return show_web(); });

	GUI_REG_LAMBDA("Editor/Clear", [this] { return show_clear_cache(); });
	GUI_REG_LAMBDA("Editor/Draw web", [this] { return true; });
	GUI_SET_ITEM_CHECKED("Editor/Draw web", is_show_web);

	auto logo = res::get_system().require_resource<res::Picture>(res::Tag::make("icons/editor_engine_logo.png"));
	gs::get_system().get_window()->set_logo(logo);
	gs::get_system().get_window()->set_title("Snake Editor");
	gui::get_system().set_show_title_bar(true);
	gui::get_system().set_show_title_bar_dbg(true);


	gs::get_system().get_window()->eventResizeWindow.subscribe([](wnd::window&, int w, int h)
	{
		//for (auto& ent : ecs::filter<scn::camera_component>())
		//{
		//	auto* camera = ecs::get_component<scn::camera_component>(ent);
		//	camera->viewport.size = glm::ivec2(w, h);
		//}
	});

	auto txt = rnd::get_system().get_texture_manager().generate_texture(res::Tag(res::Tag::memory, "__black"), {1,1}, rnd::driver::texture_header::TYPE::RGB8, {0, 0, 0});
	auto txt2 = rnd::get_system().get_texture_manager().generate_texture(res::Tag(res::Tag::memory, "__red"), {1,1}, rnd::driver::texture_header::TYPE::RGB8, {255, 0, 0});

	auto anchors = ecs::filter<scn::scene_anchor_component>();
	ecs::entity world_anchor;
	if (anchors.empty()) {
		world_anchor = ecs::create_entity();
		ecs::add_component(world_anchor, scn::scene_anchor_component{});
		ecs::add_component(world_anchor, scn::name_component{ .name = "Anchor" });
		anchors.push_back(world_anchor);
	}
	else {
		world_anchor = anchors.front();
	}
	decltype(scn::children_component::children) children;
	if (auto* kids = ecs::get_component<scn::children_component>(world_anchor)) {
		children = kids->children;
		ecs::remove_component<scn::children_component>(world_anchor);
	}
	//  camera
	{
		glm::ivec4 viewport{ glm::zero<glm::ivec2>(), gs::get_system().get_window()->get_size() };
		glm::vec3 rotation(0);
		rotation.x = -glm::radians(45.0f);
		auto ecs_entity = ecs::create_entity();
		children.push_back(ecs_entity);
		ecs::add_component(ecs_entity, scn::camera_component{ .viewport = viewport });
		ecs::add_component(ecs_entity, scn::transform_component{});
		ecs::add_component(ecs_entity, scn::is_render_component_flag{});
		ecs::add_component(ecs_entity, scn::mouse_controller_component{ .rotation = rotation });
		ecs::add_component(ecs_entity, scn::parent_component{ .parent = world_anchor });
		ecs::add_component(ecs_entity, scn::name_component{ .name = "Editor camera"});
	}
	// web
	{
		editor_web = ecs::create_entity();
		children.push_back(editor_web);
		auto web = scn::generate_web({ 50, 50 });
		web.data.materials.push_back(res::Material{ .diffuse = res::Tag(res::Tag::memory, "__black") });
		res::meshes_conteiner& data = web.data;
		res::mesh_view& mesh = web.mesh;
		mesh.material_id = 0;

		ecs::add_component(editor_web, scn::model_root_component{ .data = data });
		ecs::add_component(editor_web, scn::mesh_component{ .mesh = mesh });
		ecs::add_component(editor_web, scn::transform_component{ .local = glm::scale(glm::vec3(1, 0, 1)) });
		ecs::add_component(editor_web, scn::parent_component{ .parent = world_anchor });
		ecs::add_component(editor_web, scn::name_component{ .name = "Editor Web"});
		ecs::add_component(editor_web, rnd::render_mode_component{rnd::RENDER_MODE::LINE});
		ecs::add_component(editor_web, scn::is_render_component_flag{});
	}
	// light
	{
		light = ecs::create_entity();
		children.push_back(light);
		ecs::add_component(light, scn::light_point{
			.position = glm::vec4(50),
			.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0),
			.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0),
			.specular = glm::vec4(1)
			});

		auto m = scn::generate_sphere();
		res::meshes_conteiner& data = m.data;
		res::mesh_view& mesh = m.mesh;

		ecs::add_component(light, scn::model_root_component{ .data = data });
		ecs::add_component(light, scn::mesh_component{ .mesh = mesh });
		ecs::add_component(light, scn::transform_component{ .local = glm::translate(glm::mat4{1.0}, glm::vec3(50, 50, 50)) });
		ecs::add_component(light, scn::parent_component{ .parent = world_anchor });
		ecs::add_component(light, scn::name_component{ .name = "Light" });
	}
	// sky
	{
		sky = ecs::create_entity();
		children.push_back(sky);
		auto m = scn::generate_cube();
		ecs::add_component(sky, scn::sky_component{ .data = m.data, .mesh = m.mesh, .cube_map = std::vector<res::Tag>{
			res::Tag::make("skybox/right.jpg"),
			res::Tag::make("skybox/left.jpg"),
			res::Tag::make("skybox/bottom.jpg"),
			res::Tag::make("skybox/top.jpg"),
			res::Tag::make("skybox/back.jpg"),
			res::Tag::make("skybox/front.jpg"),
			}
			});
		ecs::add_component(sky, scn::is_render_component_flag{});
		ecs::add_component(sky, scn::parent_component{ .parent = world_anchor });
		ecs::add_component(sky, scn::name_component{ .name = "Sky" });
	}

	ecs::add_component(world_anchor, scn::children_component{ .children = children });
}


editor::EditorSystem::~EditorSystem()
{
}

void show_tree_items(ecs::entity ent)
{
	std::string obj_idx = std::to_string(ent.index);
	std::string name = "Node##" + obj_idx;
	if (auto* com_name = ecs::get_component<scn::name_component>(ent)) {
		if (!com_name->name.empty()) {
			name = com_name->name + "##" + obj_idx;
		}
	}
	static ecs::entity selected_node;

	if (ImGui::TreeNode(name.c_str()))
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("tree_context_menu");
			selected_node = ent;
		}

		if (selected_node == ent) {
			if (ImGui::BeginPopup("tree_context_menu")) {
				if (ImGui::MenuItem("Add Camera")) {
					ecs::add_component<scn::camera_component>(ent, scn::camera_component{ .viewport = glm::ivec4{100,100, 500, 500} });
				}
				ImGui::EndPopup();
			}
		}

		if (auto* anims = ecs::get_component<scn::animations_component>(ent))
		{
			std::string_view play_anim_name;
			if (auto* play = ecs::get_component<scn::playable_animation>(ent))
			{
				play_anim_name = play->name;
				ImGui::Checkbox("Is Repeat", &(play->is_repeat_animation));
			}

			std::vector<std::string> names;
			names.push_back("NONE");
			int cur = 0, i = 0;
			for (auto& anim : anims->animations)
			{
				if (play_anim_name == anim.name && cur == 0) {
					cur = i + 1;
				}
				names.push_back(anim.name);
				i++;
			}

			if (ImGui::BeginCombo(("Animations##" + obj_idx).c_str(), names[cur].c_str(), 0))
			{
				for (int n = 0; n < names.size(); ++n)
				{
					const bool is_selected = (cur == n);
					if (ImGui::Selectable(names[n].c_str(), is_selected)) {
						cur = n;

						if (auto* play = ecs::get_component<scn::playable_animation>(ent))
						{
							if (cur == 0) {
								ecs::remove_component<scn::playable_animation>(ent);
							} else {
								play->name = names[cur];
								play->current_tick = 0.f;
							}
						} else if (cur != 0){
							scn::playable_animation play_new;
							play_new.name = names[cur];
							ecs::add_component(ent, play_new);
						}
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}

		if (auto* trans = ecs::get_component<scn::transform_component>(ent))
		{
			bool is_update = false;
			eng::transform3d tr{ trans->local };

			glm::vec3 scale = tr.get_scale();
			std::string scale_name = "scale##" + obj_idx;
			ImGui::InputFloat3(scale_name.c_str(), glm::value_ptr(scale));
			if (scale != tr.get_scale()) {
				tr.set_scale(scale);
				is_update = true;
			}

			glm::vec3 translate = tr.get_pos();
			std::string translate_name = "position##" + obj_idx;
			ImGui::InputFloat3(translate_name.c_str(), glm::value_ptr(translate));
			if (translate != tr.get_pos()) {
				tr.set_pos(translate);
				is_update = true;
			}

			glm::vec3 orientation = tr.get_angles_degrees();
			glm::vec3 original_d = tr.get_angles_degrees();
			glm::quat original = tr.get_rotation();
			std::string orientation_name = "rotation##" + obj_idx;
			ImGui::InputFloat3(orientation_name.c_str(), glm::value_ptr(orientation));
			glm::quat result_orientation = glm::quat(glm::radians(orientation));
			if (std::abs(glm::dot(result_orientation, original)) < 0.999f) {
				tr.set_rotation(result_orientation);
				is_update = true;
			}

			if (is_update) {
				trans->local = tr.to_matrix();
			}
		}

		if (auto* colot = ecs::get_component<scn::light_point>(ent))
		{
			ImGui::ColorEdit3("Color", glm::value_ptr(colot->diffuse));
		}

		if (auto* camera = ecs::get_component<scn::camera_component>(ent))
		{
			ImGui::Text("fov: %3.f", camera->fov);
			ImGui::Text("near: %3.f", camera->near_distance);
			ImGui::Text("far: %3.f", camera->far_distance);
			ImGui::Text("x: %d, y: %d, width: %d, height: %d", camera->viewport.center.x, camera->viewport.center.y, camera->viewport.size.x, camera->viewport.size.y);
		}

		if (auto* children = ecs::get_component<scn::children_component>(ent))
		{
			for (auto& child : children->children)
			{
				show_tree_items(child);
			}
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

bool editor::EditorSystem::show_toolbar()
{
	ImGuiIO& io = ImGui::GetIO();
	gs::get_system().get_input_manager()->set_enabled(!io.WantCaptureMouse);

	bool is_open = true;
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

		if (ImGui::Button("Reload Texture")) {
			rnd::get_system().get_texture_manager().clear_cache();
		}
		ImGui::Separator();
		ImGui::NewLine();

		if (ImGui::CollapsingHeader("Scene Objects"))
		{
			for (ecs::entity ent : ecs::filter<scn::scene_anchor_component>())
			{
				show_tree_items(ent);
			}
		}

		ImGui::Separator();
		ImGui::NewLine();
		{
			ImGui::Text("Camera");
			ImGui::Separator();
			std::vector<std::string> names;
			int cam_id = 1;
			int cam_cur = 0;
			int cam_cur_id = 0;
			auto cameras = ecs::filter<scn::camera_component>();
			for (auto& ent : cameras)
			{
				if (auto* name = ecs::get_component<scn::name_component>(ent)) {
					names.push_back(name->name);
				} else {
					names.push_back("Camera" + std::to_string(cam_id++));
				}

				if (auto* visible = ecs::get_component<scn::is_render_component_flag>(ent)) {
					cam_cur_id = cam_cur;
				}

				++cam_cur;
			}

			if (ImGui::BeginCombo("Current Camera", names[cam_cur_id].c_str(), 0))
			{
				for (int n = 0; n < names.size(); ++n)
				{
					const bool is_selected = (cam_cur_id == n);
					if (ImGui::Selectable(names[n].c_str(), is_selected)) {
						auto old = cameras[cam_cur_id];
						auto new_one = cameras[n];
						cam_cur_id = n;
						ecs::add_component(new_one, scn::is_render_component_flag{});
						ecs::remove_component<scn::is_render_component_flag>(old);
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			for (auto& ent : ecs::filter<scn::camera_component, scn::is_render_component_flag>()) {
				eng::transform3d ct{ glm::mat4{1.0} };
				if (auto* trans = ecs::get_component<scn::transform_component>(ent)) {
					ct = eng::transform3d{ trans->local };
				}
				ImGui::Text("pitch: %.3f, yaw: %.3f, roll: %.3f", glm::degrees(ct.get_pitch()), glm::degrees(ct.get_yaw()), glm::degrees(ct.get_roll()));
				auto pos = ct.get_pos();
				ImGui::Text("x: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);
			}
		}

		ImGui::Text("Scene");
		ImGui::Separator();

		static int model_load_method = 0;
		ImGui::RadioButton("Combo models", &model_load_method, 0); ImGui::SameLine();
		ImGui::RadioButton("Input model", &model_load_method, 1);

		gs::get_system().check_loaded_model();
		if (ImGui::Button("Add model")) {
			gs::get_system().load_model(model_load_method == 0 ? models_list[current_model].c_str() : buf);
		} ImGui::SameLine();

		if (model_load_method == 0) {
			if (ImGui::BeginCombo("Models", models_list[current_model].c_str()))
			{
				for (int n = 0; n < models_list.size(); n++)
				{
					const bool is_selected = (current_model == n);
					if (ImGui::Selectable(models_list[n].c_str(), is_selected))
						current_model = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		} else {
			ImGui::InputText("Name", buf, 64);
		}

		static int cube_count_add = 1;
		static int cube_count_remove = 1;
		static int cube_count = 0;

		if (ImGui::Button("add cube")) {
			for (int i = 0; i < cube_count_add; ++i) {
				//gs::get_system().add_cube_to_scene(20.f);
				cube_count++;
			}
		}		
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SliderInt("count add", &cube_count_add, 1, 100);

		if (ImGui::Button("remove cube")) {
			for (int i = 0; i < cube_count_remove; ++i) {
				//gs::get_system().remove_cube();
				if (cube_count > 0)
					cube_count--;
			}
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SliderInt("count remove", &cube_count_remove, 1, 100); 
		ImGui::Text("cubes count: %d", cube_count);

		ImGui::Separator();
		if (ImGui::BeginCombo("Render mode", render_modes_list[current_render_mode].c_str(), 0))
		{
			for (int n = 0; n < render_modes_list.size(); n++)
			{
				const bool is_selected = (current_render_mode == n);
				if (ImGui::Selectable(render_modes_list[n].c_str(), is_selected)) {
					current_render_mode = n; 
					rnd::get_system().set_render_mode(mmap[render_modes_list[n]]);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		eng::transform3d ct;
		ImGui::Text("pitch: %.3f, yaw: %.3f, roll: %.3f", glm::degrees(ct.get_pitch()), glm::degrees(ct.get_yaw()), glm::degrees(ct.get_roll()));
		auto pos = ct.get_pos();
		ImGui::Text("x: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);

	}
	ImGui::End();
	return is_open;
}

bool editor::EditorSystem::show_web()
{
	const bool cur_is_show = GUI_IS_ITEM_CHECKED("Editor/Draw web");
	if (!cur_is_show && cur_is_show != is_show_web) {
		ecs::remove_component<scn::is_render_component_flag>(editor_web);
		//ecs::remove_component<scn::is_render_component_flag>(sky);
	}
	else if (cur_is_show && cur_is_show != is_show_web){
		ecs::add_component(editor_web, scn::is_render_component_flag{});
		//ecs::add_component(sky, scn::is_render_component_flag{});
	} 

	is_show_web = cur_is_show;
	return true;
}

bool editor::EditorSystem::show_scene()
{
	bool is_open = true;
	if (ImGui::Begin("Scene", &is_open))
	{
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		for (auto& ent : ecs::filter<scn::camera_component>())
		{
			auto* camera = ecs::get_component<scn::camera_component>(ent);
			camera->viewport.size = glm::ivec2(contentRegionAvailable.x, contentRegionAvailable.y);
		}

		if (ImGui::BeginChild("Buttons", ImVec2(0, 60), false, ImGuiWindowFlags_NoScrollbar)) { 
			if (ImGui::Button("Button 1")) {
				egLOG("", "Click btn1");
			}
			if (ImGui::Button("Button 2")) {
				egLOG("", "Click btn2");
			}
			ImGui::EndChild(); 
		}

		auto texture = rnd::get_system().get_texture_manager().find(res::Tag(res::Tag::memory, "__color_scene_rt"));
		auto* backend = wnd::get_system().get_gui_backend();

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 original_button_color = style.Colors[ImGuiCol_Button];
		ImVec4 original_button_hovered_color = style.Colors[ImGuiCol_ButtonHovered];
		ImVec4 original_button_active_color = style.Colors[ImGuiCol_ButtonActive];
		ImVec2 original_padding = style.FramePadding;
		style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.FramePadding = { 0, 0 };

		ImGui::SetCursorScreenPos(pos);
		ImGui::ImageButton("##ViewScene", backend->get_imgui_texture_from_texture(texture), contentRegionAvailable, ImVec2(0, 1), ImVec2(1, 0));
		
		style.Colors[ImGuiCol_Button] = original_button_color;
		style.Colors[ImGuiCol_ButtonHovered] = original_button_hovered_color;
		style.Colors[ImGuiCol_ButtonActive] = original_button_active_color;
		style.FramePadding = original_padding;

		if (ImGui::IsItemHovered()) {
			gs::get_system().get_input_manager()->unblock_layer_once();
		}

	}
	ImGui::End();

	return is_open;
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

bool editor::EditorSystem::show_clear_cache()
{
	bool is_open = true;
	if (ImGui::Begin("Caches", &is_open))
	{
		if (ImGui::Button("Reload Shaders")) {
			gs::get_system().reload_shaders();
		}

		if (ImGui::Button("Reload Texture")) {
			rnd::get_system().get_texture_manager().clear_cache();
		}
	}
	ImGui::End();
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
	trn2->add_yaw(45.f);

	return true;
}
