#include "game_system.h"

#include "../input/inp_input_manager.h"
#include "../input/inp_input_system.h"

#include "../common/enums.h"
#include "../scene/cude.h"
#include "../scene/model.h"

#include "../windows/window_system.h"
#include "../resource/res_resource_system.h"

game::GameSystem* p_game_system = nullptr;

game::GameSystem& game::get_system()
{
	ASSERT_MSG(p_game_system, "Game system is nullptr!");
	return *p_game_system;
}

game::GameSystem::GameSystem()
{
	wnd::WindowSystem& wndCreator = wnd::get_system();

	window = wndCreator.make_window("Window 3.0", WIDTH, HEIGHT);
	input = std::make_shared<inp::InputManager>();
	reload_shaders();

	NormalVisualizeShader = Shader::load("normal.vert", "normal.frag", "normal.geom");

	init_cude();

	camera = std::make_shared<Camera>(input, glm::vec3(0, 0, 2), glm::radians(45.0f));
	camera->attath_to_window(window);

	input->create_click_action(inp::KEYBOARD_BUTTONS::ESCAPE, [this] { window->set_should_close(true); });
	input->create_click_action(inp::KEYBOARD_BUTTONS::TAB, [this] { camera->set_enabled(!camera->is_enabled()); window->set_cursor_mode(camera->is_enabled() ? CursorMode::Disable : CursorMode::Normal); });
}

game::GameSystem::~GameSystem()
{
}

void game::GameSystem::capture()
{
	camera->update();
}

void game::GameSystem::render()
{
	if (!ourShader) {
		return;
	}

	// activate shader
	ourShader->use();

	glm::mat4 projection = camera->projection(glm::radians(45.f));
	glm::mat4 view = camera->view();
	ourShader->uniform_matrix("projection", projection);
	ourShader->uniform_matrix("view", view);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	ourShader->uniform_matrix("model", model);
	ourModel->Draw(*ourShader.get());

	if (is_show_normal && NormalVisualizeShader)
	{
		NormalVisualizeShader->use();
		NormalVisualizeShader->uniform_matrix("projection", projection);
		NormalVisualizeShader->uniform_matrix("view", view);
		NormalVisualizeShader->uniform_matrix("model", model);
		ourModel->Draw(*NormalVisualizeShader.get());
	}

	//model = glm::scale(model, glm::vec3(cube_scale));
	//ourShader->uniform_matrix("model", model);

	//ourShader->uniform_float("time", window->current_time());

	//draw_cude(*ourShader.get());
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
	ourModel = std::make_shared<scene::Model>(path);

}

void game::GameSystem::reload_shaders()
{
	ourShader = Shader::load("scene.vert", /*"lines.frag"*/"scene.frag", "scene.geom");
}

