#include "gs_game_system.h"

#include <inp_input_manager.h>
#include <inp_input_system.h>

#include <enums.h>
#include <scn_primitives.h>
#include <scn_model.h>

#include <wnd_window_system.h>
#include <res_resource_system.h>

#include <rnd_render_system.h>
#include <timer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

game::GameSystem* p_game_system = nullptr;

game::GameSystem& game::get_system()
{
	ASSERT_MSG(p_game_system, "Game system is nullptr!");
	return *p_game_system;
}

game::GameSystem::GameSystem()
{
	wnd::WindowSystem& wndCreator = wnd::get_system();

	window = wndCreator.get_active_window();
	input = std::make_shared<inp::InputManager>();

	camera = std::make_shared<snakeengine::MouseCamera>(glm::vec3(4, 15, 80), window->get_size());
	camera->look_at(glm::vec3{ 0 });
	camera->enable_input_actions(input);
	window->eventResizeWindow.subscribe([this](wnd::window&, int w, int h) { camera->on_viewport_size_change(glm::ivec2{ w, h }); });

	input->create_click_action(inp::KEYBOARD_BUTTONS::ESCAPE, [this](float) { window->shutdown(); });
	input->create_click_action(inp::KEYBOARD_BUTTONS::TAB, [this](float) {
		camera->set_enabled(!camera->is_enabled()); 
		window->set_cursor_mode(camera->is_enabled() ? CursorMode::Disable : CursorMode::Normal); 
		});
}

game::GameSystem::~GameSystem()
{
	camera->disable_input_actions(input);
}

void game::GameSystem::capture()
{
}

void game::GameSystem::prepair_render()
{
	if (window->is_minimize_mode()) {
		return;
	}

	rnd::get_system().clear(render::driver::CLEAR_FLAGS::COLOR_BUFFER);
	rnd::get_system().clear(render::driver::CLEAR_FLAGS::DEPTH_BUFFER);

	rnd::GlobalUniform val;

	val.projection = camera->projection();
	val.view = camera->view();
	val.time = (float)Timer::now();

	rnd::get_system().get_sh_manager().update_global_uniform(val);

}

void game::GameSystem::render()
{
	if (window->is_minimize_mode()) {
		return;
	}

	// render the loaded model
	for (auto& model : scene_objects) {
		rnd::get_system().get_renderer().draw(model);
	}

	// temporary off while geom shader dont use
	if (is_show_normal && ourModel)
	{		
		rnd::get_system().get_renderer().draw(*ourModel);
		//for (auto& model : scene_objects) {
		//	rnd::get_system().get_sh_manager().uniform("normal", "model", glm::scale(model.model, glm::vec3(cube_scale)));
		//	model.draw(rnd::get_system().get_sh_manager().use("normal"));
		//}
	}
}

void game::GameSystem::begin_frame()
{
	window->update_frame();
	input->notify_listeners(window->get_delta());
}

void game::GameSystem::end_frame()
{
	window->on_update();
}

void game::GameSystem::set_enable_input(bool enable)
{
	input->set_enabled(enable);
	window->set_cursor_mode(camera->is_enabled() ? CursorMode::Disable : CursorMode::Normal);
}

void game::GameSystem::load_model(std::string_view path)
{
	ourModel = std::make_shared<scene::Model>(path);
}

void game::GameSystem::reload_shaders()
{
	rnd::get_system().get_sh_manager().clear_cache();
}

void game::GameSystem::add_cube_to_scene(float radius)
{
	static bool is_gen_cube = true;
	scene_objects.push_back(is_gen_cube ? generate_cube() : generate_sphere());
	is_gen_cube = !is_gen_cube;
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

