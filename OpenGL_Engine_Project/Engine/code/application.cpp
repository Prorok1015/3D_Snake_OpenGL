#include "application.h"
#include "debug_ui/debug_ui_init.h"

#include "windows/window.h"
#include "input/events.h"

#include "ui/ui_system.h"
#include "common/enums.h"

#include "scene/cude.h"
#include "scene/model.h"

application::Application::Application()
{
	display.initialize("Window 3.0", WIDTH, HEIGHT);
	dbg_ui::initialize(display.window);

	ourShader = Shader::load("./res/scene.glslv", "./res/scene.glslf");
	camera = std::make_unique<Camera>(display, glm::vec3(0, 0, 2), glm::radians(90.0f));
	ourModel = std::make_unique<scene::Model>("D:/MyProject/SnakeProject/OpenGL_Engine_Project/Engine/res/objects/backpack/backpack.obj");

	display.input->listeners[KEYBOARD::ESCAPE] += [this] { if (!display.input->jpressed(KEYBOARD::ESCAPE)) return; display.window->set_should_close(true); };
	display.input->listeners[KEYBOARD::TAB] += [this] { if (!display.input->jpressed(KEYBOARD::TAB)) return; display.input->toogle_cursor(); };
}

application::Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Window::terminate();
}

int application::Application::init_libraris()
{
	Window::init_libraris();
	return 0;
}

int application::Application::Run()
{
	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float camX = 0.0f;
	float camY = 0.0f;

	while (!display.window->is_should_close()) {
		display.window->update_frame();
		display.input->poll_listeners();

		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			// 3. Show another simple window.
			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
			// Rendering
			ImGui::Render();
		}

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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		display.window->swap_buffers();
		display.input->poll_events();
	}

	return 0;
}
