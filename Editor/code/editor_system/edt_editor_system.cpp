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
	dbg_ui::get_system().set_show_title_bar(true);

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

	auto txt = rnd::get_system().get_texture_manager().generate_texture(res::Tag("memory", "__black"), {1,1}, rnd::driver::texture_header::TYPE::RGB8, {0, 0, 0});

	scn::Model web = generate_web({ 50, 50 });
	web.meshes.back().material.diffuse = res::Tag("memory", "__black");
	web.model = glm::scale(web.model, glm::vec3(20, 0, 20));

	editor_web = ecs::create_entity();
	ecs::add_component(editor_web, scn::model_comonent{ web.meshes });
	ecs::add_component(editor_web, scn::transform_component{ web.model });
	ecs::add_component(editor_web, scn::is_render_component_flag{});
	ecs::add_component(editor_web, rnd::render_mode_component{rnd::RENDER_MODE::LINE});

	light = ecs::create_entity();
	ecs::add_component(light, rnd::light_point{ 
		.position = glm::vec4(50), 
		.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0),
		.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0),
		.specular = glm::vec4(1)
		});
	ecs::add_component(light, scn::transform_component{ .world = glm::translate(glm::mat4{1.0}, glm::vec3(50, 50, 50)) });
	scn::Model m = generate_sphere();
	ecs::add_component(light, scn::model_comonent{ m.meshes });
	ecs::add_component(light, scn::is_render_component_flag{});

	sky = ecs::create_entity();
	ecs::add_component(sky, rnd::cubemap_component{ m.meshes.front(), std::vector<res::Tag>{
		res::Tag::make("skybox/right.jpg"),
		res::Tag::make("skybox/left.jpg"),
		res::Tag::make("skybox/bottom.jpg"),
		res::Tag::make("skybox/top.jpg"),
		res::Tag::make("skybox/back.jpg"),
		res::Tag::make("skybox/front.jpg"),
		}
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
	ImGuiIO& io = ImGui::GetIO();
	gs::get_system().get_input_manager()->set_enabled(!io.WantCaptureMouse);

	bool is_open = true;
	float height = gs::get_system().get_window()->get_size().y - 80;
	ImGui::SetNextWindowSize(ImVec2{ 400, height});
	ImGui::SetNextWindowPos(ImVec2{ 0, 80 });
	if (ImGui::Begin("Observer", &is_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
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
			for (ecs::entity ent : ecs::filter<scn::model_comonent>())
			{
				std::string obj_idx = std::to_string(ent.index);
				std::string obj_name = "Object##" + obj_idx;
				if (ImGui::TreeNode(obj_name.c_str()))
				{
					scn::transform_component* trans = ecs::get_component<scn::transform_component>(ent);
					if (trans) {
						eng::transform3d tr{ trans->world };
						glm::vec3 scale = tr.get_scale();
						std::string scale_name = "scale##" + obj_idx;
						ImGui::InputFloat(scale_name.c_str(), glm::value_ptr(scale));
						if (scale != tr.get_scale()) {
							tr.set_scale(glm::vec3{ scale.x });
							trans->world = tr.to_matrix();
						}
					}

					if (scn::model_comonent* model = ecs::get_component<scn::model_comonent>(ent))
					{
						std::string mesh_capter = "Meshes"s + " (" + std::to_string(model->meshes.size()) + ")" + "##" + obj_idx ;
						if (ImGui::TreeNode(mesh_capter.c_str()))
						{
							int idx = 0;
							for (res::Mesh& mesh : model->meshes)
							{
								std::string mesh_idx = std::to_string(++idx);
								std::string mesh_name = "Mesh#" + obj_idx + "." + mesh_idx;
								if (ImGui::TreeNode(mesh_name.c_str()))
								{
									res::Material& mat = mesh.material;
									static char diffuce_buf[64];
									if (mat.diffuse.is_valid()) {
										ImGui::Text(mat.diffuse.get_full().data());
									}
									ImGui::InputText("diffuse", diffuce_buf, 64);
									if (ImGui::Button("Submit diffuse")) {
										res::Tag new_diffuse = res::Tag::make(diffuce_buf);
										mat.diffuse = new_diffuse;
									}

									static char ambient_buf[64];
									if (mat.ambient.is_valid()) {
										ImGui::Text(mat.ambient.get_full().data());
									}
									ImGui::InputText("ambient", ambient_buf, 64);
									if (ImGui::Button("Submit ambient")) {
										res::Tag new_diffuse = res::Tag::make(ambient_buf);
										mat.ambient = new_diffuse;
									}

									static char normal_buf[64];
									if (mat.normal.is_valid()) {
										ImGui::Text(mat.normal.get_full().data());
									}
									ImGui::InputText("normal", normal_buf, 64);
									if (ImGui::Button("Submit normal")) {
										res::Tag new_diffuse = res::Tag::make(normal_buf);
										mat.normal = new_diffuse;
									}

									static char specular_buf[64];
									if (mat.specular.is_valid()) {
										ImGui::Text(mat.specular.get_full().data());
									}
									ImGui::InputText("specular", specular_buf, 64);
									if (ImGui::Button("Submit specular")) {
										res::Tag new_diffuse = res::Tag::make(specular_buf);
										mat.specular = new_diffuse;
									}

									ImGui::TreePop();
									ImGui::Spacing();
								}
							} // mesh

							ImGui::TreePop();
							ImGui::Spacing();
						}
					}
					ImGui::TreePop();
					ImGui::Spacing();
				} // object
			}
		}

		ImGui::Separator();
		ImGui::NewLine();
		ImGui::Text("Light");
		ImGui::Separator();
		{
			for (auto& ent : ecs::filter<rnd::light_point, scn::transform_component>()) {
				ImGui::ColorEdit3("Light color", glm::value_ptr(ecs::get_component<rnd::light_point>(ent)->diffuse));

				eng::transform3d ct{ ecs::get_component<scn::transform_component>(ent)->world };
				auto pos = ct.get_pos();
				glm::vec3 vec4f = ct.get_pos();
				ImGui::InputFloat3("light position", glm::value_ptr(vec4f));

				if (vec4f != ct.get_pos()) {
					ct.set_pos(vec4f);
					ecs::get_component<scn::transform_component>(ent)->world = ct.to_matrix();
					ecs::get_component<rnd::light_point>(ent)->position = glm::vec4(ct.get_pos(), 1.0);
				}

				ImGui::Text("pitch: %.3f, yaw: %.3f, roll: %.3f", glm::degrees(ct.get_pitch()), glm::degrees(ct.get_yaw()), glm::degrees(ct.get_roll()));
				ImGui::Text("x: %.3f, y: %.3f, z: %.3f", pos.x, pos.y, pos.z);
			}
		}

		ImGui::Separator();
		ImGui::NewLine();
		{
			ImGui::Text("Camera");
			ImGui::Separator();
			if (ImGui::BeginCombo("Cameras", cameras_list[current_camera].c_str(), 0))
			{
				for (int n = 0; n < cameras_list.size(); n++)
				{
					const bool is_selected = (current_camera == n);
					if (ImGui::Selectable(cameras_list[n].c_str(), is_selected)) {
						current_camera = n;
						if (current_camera == 0) {
							ecs::add_component(camera->ecs_entity, scn::is_render_component_flag{});
							ecs::remove_component<scn::is_render_component_flag>(second_camera->ecs_entity);
							second_camera_controller->disable_input_actions(gs::get_system().get_input_manager());
							camera_controller->enable_input_actions(gs::get_system().get_input_manager());
						}
						else {
							ecs::remove_component<scn::is_render_component_flag>(camera->ecs_entity);
							ecs::add_component(second_camera->ecs_entity, scn::is_render_component_flag{});
							camera_controller->disable_input_actions(gs::get_system().get_input_manager());
							second_camera_controller->enable_input_actions(gs::get_system().get_input_manager());
						}
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
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
				gs::get_system().add_cube_to_scene(20.f);
				cube_count++;
			}
		}		
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SliderInt("count add", &cube_count_add, 1, 100);

		if (ImGui::Button("remove cube")) {
			for (int i = 0; i < cube_count_remove; ++i) {
				gs::get_system().remove_cube();
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

		ImGui::End();
	}

	return is_open;
}

bool editor::EditorSystem::show_web()
{
	const bool cur_is_show = DBG_UI_IS_ITEM_CHECKED("EDITOR/SHOW_WEP");
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
