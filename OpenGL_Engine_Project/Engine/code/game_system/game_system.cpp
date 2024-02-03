#include "game_system.h"
#include "../debug_ui/debug_ui_init.h"
#include "../input/events.h"

#include "../common/enums.h"

#include "../scene/cude.h"
#include "../scene/model.h"

using namespace application;

GameSystem::GameSystem()
{
	display.initialize("Window 3.0", WIDTH, HEIGHT);

	ourShader = Shader::load("./res/scene.glslv", "./res/scene.glslf");
	camera = std::make_unique<Camera>(display, glm::vec3(0, 0, 2), glm::radians(90.0f));
	ourModel = std::make_unique<scene::Model>("D:/MyProject/SnakeProject/OpenGL_Engine_Project/Engine/res/objects/backpack/backpack.obj");

	display.input->listeners[KEYBOARD::ESCAPE] += [this] { if (!display.input->jpressed(KEYBOARD::ESCAPE)) return; display.window->set_should_close(true); };
	display.input->listeners[KEYBOARD::TAB] += [this] { if (!display.input->jpressed(KEYBOARD::TAB)) return; display.input->toogle_cursor(); };
}

GameSystem::~GameSystem()
{
}

void GameSystem::Capture()
{
	if (display.input->_cursor_locked) {
		camY += -display.input->deltaY / display.window->height * 2;
		camX += -display.input->deltaX / display.window->width * 2;

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

void GameSystem::Render()
{
	// activate shader
	ourShader->use();

	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)display.window->width / (float)display.window->height, 0.1f, 100.0f);
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

void GameSystem::BeginFrame()
{
	display.window->update_frame();
	display.input->poll_listeners();

}

void GameSystem::EndFrame()
{
	display.window->swap_buffers();
	display.input->poll_events();
}
