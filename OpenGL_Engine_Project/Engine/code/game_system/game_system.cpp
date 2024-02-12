#include "game_system.h"
#include "../debug_ui/debug_ui_init.h"
#include "../input/inp_input_manager.h"

#include "../common/enums.h"

#include "../scene/cude.h"
#include "../scene/model.h"

#include "../input/inp_input_actions.h"

using namespace application;

GameSystem::GameSystem()
{
	display.initialize("Window 3.0", WIDTH, HEIGHT);

	ourShader = Shader::load("./res/scene.glslv", "./res/scene.glslf");
	camera = std::make_unique<Camera>(display, glm::vec3(0, 0, 2), glm::radians(90.0f));
	ourModel = std::make_unique<scene::Model>("D:/MyProject/SnakeProject/OpenGL_Engine_Project/Engine/res/objects/backpack/backpack.obj");

	auto Escape = inp::InputActionClick::create(inp::KEYBOARD_BUTTONS::ESCAPE, [this] { display.window->set_should_close(true); });
	display.input->registrate(Escape);

	auto Tab = inp::InputActionClick::create(inp::KEYBOARD_BUTTONS::TAB, [this] { camera->set_enable(!camera->is_enable()); display.window->set_cursor_mode(camera->is_enable() ? CursorMode::Disable : CursorMode::Normal); });
	display.input->registrate(Tab);
}

GameSystem::~GameSystem()
{
}

void GameSystem::capture()
{
	camera->update();
}

void GameSystem::render()
{
	// activate shader
	ourShader->use();

	glm::mat4 projection = camera->projection(glm::radians(45.f));//glm::perspective(glm::radians(45.f), display.window->get_aspect_ratio(), 0.1f, 100.0f);
	glm::mat4 view = camera->view();
	ourShader->uniform_matrix("projection", projection);
	ourShader->uniform_matrix("view", view);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	ourShader->uniform_matrix("model", model);
	ourModel->Draw(*ourShader.get());
}

void GameSystem::begin_frame()
{
	display.window->update_frame();
	float dt = display.window->delta;
	display.input->poll_listeners(dt);
}

void GameSystem::end_frame()
{
	display.window->swap_buffers();
}
