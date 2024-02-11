#include "game_system.h"
#include "../debug_ui/debug_ui_init.h"
#include "../input/events.h"

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

	auto Tab = inp::InputActionClick::create(inp::KEYBOARD_BUTTONS::TAB, [this] { display.input->toogle_cursor(); });
	display.input->registrate(Tab);
}

GameSystem::~GameSystem()
{
}

void GameSystem::capture()
{
	if (display.input->_cursor_locked) {
		camY += -display.input->deltaY / display.window->height_ * 2;
		camX += -display.input->deltaX / display.window->width_ * 2;

		float delta = display.window->delta;

		if (camY < -glm::radians(89.0f)) {
			camY = -glm::radians(89.0f);
		}
		if (camY > glm::radians(89.0f)) {
			camY = glm::radians(89.0f);
		}

		camera->rotation = glm::mat4(1.0f);
		camera->rotate(camY, camX, 0);
		std::cout << "{ " << camX << ", " << camY << " }; \t";
		std::cout << "[ " << camera->position.x << ", " << camera->position.y << " ]\n";
	}
}

void GameSystem::render()
{
	// activate shader
	ourShader->use();

	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)display.window->width_ / (float)display.window->height_, 0.1f, 100.0f);
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
	display.input->poll_events();
}
