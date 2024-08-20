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

gs::GameSystem* p_game_system = nullptr;

gs::GameSystem& gs::get_system()
{
	ASSERT_MSG(p_game_system, "Game system is nullptr!");
	return *p_game_system;
}

gs::GameSystem::GameSystem()
{
	window = wnd::get_system().get_active_window();
	input = std::make_shared<inp::InputManager>();
	inp::get_system().activate_manager(input);

	camera = std::make_shared<snakeengine::MouseCamera>(glm::vec3(4, 15, 80), window->get_size());
	camera->look_at(glm::vec3{ 0 });
	camera->enable_input_actions(input);

	renderer = std::make_shared<renderer_3d>();
	renderer->camera = camera.get();
	rnd::get_system().activate_renderer(renderer);

	window->eventResizeWindow.subscribe([this](wnd::window&, int w, int h) { camera->on_viewport_size_change(glm::ivec2{ w, h }); });

	input->create_click_action(inp::KEYBOARD_BUTTONS::ESCAPE, [this](float) { window->shutdown(); });
	input->create_click_action(inp::KEYBOARD_BUTTONS::TAB, [this](float) {
		camera->set_enabled(!camera->get_is_enabled()); 
		window->set_cursor_mode(camera->get_is_enabled() ? CursorMode::Disable : CursorMode::Normal); 
	});
}

gs::GameSystem::~GameSystem()
{
	camera->disable_input_actions(input);
	rnd::get_system().deactivate_renderer(renderer);
	inp::get_system().deactivate_manager(input);
}

void gs::GameSystem::set_enable_input(bool enable)
{
	input->set_enabled(enable);
	window->set_cursor_mode(camera->get_is_enabled() ? CursorMode::Disable : CursorMode::Normal);
}

void gs::GameSystem::load_model(std::string_view path)
{
	renderer->scene_objects.push_back(scene::Model(path));
	auto rand_pos = glm::diskRand(20.f);

	auto& m = renderer->scene_objects.back();
	m.model = glm::translate(m.model, glm::vec3{ rand_pos.x, 1.f, rand_pos.y });
	m.model = glm::scale(m.model, glm::vec3{ std::abs(rand_pos.x) });
}

void gs::GameSystem::reload_shaders()
{
	rnd::get_system().get_shader_manager().clear_cache();
}

void gs::GameSystem::add_cube_to_scene(float radius)
{
	static bool is_gen_cube = true;
	renderer->scene_objects.push_back(is_gen_cube ? generate_cube() : generate_sphere());
	is_gen_cube = !is_gen_cube;
	auto rand_pos = glm::diskRand(radius);

	auto& m = renderer->scene_objects.back();
	m.model = glm::translate(m.model, glm::vec3{ rand_pos.x, 1.f, rand_pos.y });
}

void gs::GameSystem::remove_cube()
{
	if (renderer->scene_objects.empty()) {
		return;
	}
	renderer->scene_objects.pop_back();
}

