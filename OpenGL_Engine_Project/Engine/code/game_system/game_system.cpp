#include "game_system.h"

#include "../input/inp_input_manager.h"
#include "../input/inp_input_system.h"

#include "../common/enums.h"
#include "../scene/primitives.h"
#include "../scene/model.h"

#include "../windows/window_system.h"
#include "../resource/res_resource_system.h"

#include "../render/rnd_render_system.h"

game::GameSystem* p_game_system = nullptr;

game::GameSystem& game::get_system()
{
	ASSERT_MSG(p_game_system, "Game system is nullptr!");
	return *p_game_system;
}

scene::Model g_Scene;

game::GameSystem::GameSystem()
{
	wnd::WindowSystem& wndCreator = wnd::get_system();

	window = wndCreator.make_window("Window 3.0", WIDTH, HEIGHT);
	input = std::make_shared<inp::InputManager>();
	rnd::get_system().get_sh_manager().init_global_uniform();

	camera = std::make_shared<Camera>(input, glm::vec3(-5, 5, 0), glm::radians(45.0f));
	camera->attath_to_window(window);

	input->create_click_action(inp::KEYBOARD_BUTTONS::ESCAPE, [this] { window->set_should_close(true); });
	input->create_click_action(inp::KEYBOARD_BUTTONS::TAB, [this] { camera->set_enabled(!camera->is_enabled()); window->set_cursor_mode(camera->is_enabled() ? CursorMode::Disable : CursorMode::Normal); });
	
	g_Scene = generate_network({50, 50});

	auto txt = rnd::get_system().get_txr_manager().generate_texture(res::Tag::make("__black"), { 1,1 }, 3, {0, 0, 0});
	txt->tmp_type = "texture_diffuse";

	g_Scene.meshes.back().textures.push_back(txt);
	g_Scene.model = glm::scale(g_Scene.model, glm::vec3(20, 0, 20));
}

game::GameSystem::~GameSystem()
{
	g_Scene.meshes.clear();
}

void game::GameSystem::capture()
{
	camera->update();
}

void game::GameSystem::render()
{
	if (window->aspect_ratio() < 0.01) {
		return;
	}

	rnd::GlobalUniform val;

	val.projection = camera->projection(glm::radians(45.f));
	val.view = camera->view();
	val.time = window->current_time();

	rnd::get_system().get_sh_manager().update_global_uniform(val);

	auto tmp = rnd::get_system().render_view();
	rnd::get_system().render_view(rnd::RENDER_VIEW::LINE);
	rnd::get_system().set_line_size(2);
	rnd::get_system().get_sh_manager().uniform("scene"/*"scene_network"*/, "model", g_Scene.model);
	g_Scene.draw(rnd::get_system().get_sh_manager().use("scene"/*"scene_network"*/));
	rnd::get_system().render_view(tmp);

	// render the loaded model
	for (auto& model : scene_objects) {
		rnd::get_system().get_sh_manager().uniform("scene", "model", glm::scale(model.model, glm::vec3(cube_scale)));
		model.draw(rnd::get_system().get_sh_manager().use("scene"));
	}

	// temporary off while geom shader dont use
	if (is_show_normal && false)
	{
		for (auto& model : scene_objects) {
			rnd::get_system().get_sh_manager().uniform("normal", "model", glm::scale(model.model, glm::vec3(cube_scale)));
			model.draw(rnd::get_system().get_sh_manager().use("normal"));
		}
	}
}

void game::GameSystem::begin_frame()
{
	window->update_frame();
	float dt = window->delta;
	input->notify_listeners(dt);
}

void game::GameSystem::end_frame()
{
	window->swap_buffers();
}

void game::GameSystem::switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action)
{

}

void game::GameSystem::set_enable_input(bool enable)
{
	input->set_enabled(enable);
	window->set_cursor_mode(camera->is_enabled() ? CursorMode::Disable : CursorMode::Normal);
}

void game::GameSystem::load_model(std::string_view path)
{
	//ourModel = std::make_shared<scene::Model>(path);

}

void game::GameSystem::reload_shaders()
{
	rnd::get_system().get_sh_manager().clear_cache();
}

void game::GameSystem::add_cube_to_scene(float radius)
{
	scene_objects.push_back(generate_cube());
	auto rand_pos = glm::diskRand(radius);

	auto& m = scene_objects.back();
	m.model = glm::translate(m.model, glm::vec3{ rand_pos.x, 1.f, rand_pos.y });
}

void game::GameSystem::remove_cube()
{
	if (scene_objects.empty()) {
		return;
	}
	scene_objects.pop_back();
}

