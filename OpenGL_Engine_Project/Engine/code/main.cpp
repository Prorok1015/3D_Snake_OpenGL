#include "common/common.h"

#include "windows/window.h"
#include "input/events.h"
#include "render/shader/shader.h"
#include "render/texture/texture.h"

#include "render/camera/camera.h"
#include "ui/shape.h"
#include "ui/bitmap.h"

#include "ui/ui_system.h"
#include "common/enums.h"
#include "scene/cude.h"
#include "scene/model.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

int main()
{
	Window::initialize("Window 3.0", WIDTH, HEIGHT);
	Events::initialize();
	UiSystem::initialize();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(Window::window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::unique_ptr<Shader> ourShader = Shader::load("./res/scene.glslv", "./res/scene.glslf");
	if (!ourShader) {
		Window::terminate();
		return 1;
	}

	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::unique_ptr<Camera> camera = std::make_unique<Camera>(glm::vec3(0, 0, 2), glm::radians(90.0f));

	float camX = 0.0f;
	float camY = 0.0f;

	//Shape sh;
	//sh.pos = { 0, HEIGHT - 60, 0 };
	//sh.size = { WIDTH, 60 };
	//sh.color.r = 0.5;

	//init_cude();

	//Bitmap bmp;
	//bmp.size = { 512, 512 };
	//bmp.load_texture("./res/test.jpg");

	//int speedShape = 100;
	//float moveX = 0;
	//float moveY = 0;
	//bool isClk = false;
	scene::Model ourModel("D:/MyProject/SnakeProject/OpenGL_Engine_Project/Engine/res/objects/backpack/backpack.obj");

	Events::listeners[KEYBOARD::ESCAPE] += [] { if (!Events::jpressed(KEYBOARD::ESCAPE)) return; Window::set_should_close(true); };
	Events::listeners[KEYBOARD::TAB] += [] { if (!Events::jpressed(KEYBOARD::TAB)) return; Events::toogle_cursor(); };

	while (!Window::is_should_close()) {
		Window::update_frame();
		Events::poll_listeners();

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

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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

		//if (Events::jclicked(0)) {
		//	moveX = Events::x;
		//	moveY = Events::y;
		//	if (isClk) {
		//		sh.color.r = 0.f;
		//		sh.color.g = 1.f;
		//	} else {
		//		sh.color.r = 1.f;
		//		sh.color.g = 0.f;
		//	}
		//	isClk = !isClk;
		//}

		//glm::fvec2 forw = { moveX - sh.pos.x, moveY - sh.pos.y };
		//forw = glm::normalize(forw);

		//if (sh.pos.x != moveX) {
		//	sh.pos.x += speedShape * delta * forw.x;
		//}
		//if (sh.pos.y != moveY) {
		//	sh.pos.y += speedShape * delta * forw.y;
		//}

		//if (sh.pos.x + sh.size.x > Window::width) {
		//	sh.pos.x = Window::width - sh.size.x;
		//}
		//if (sh.pos.y + sh.size.y > Window::height) {
		//	sh.pos.y = Window::height - sh.size.y;
		//}

		//bmp.prepare_data();

		if (Events::_cursor_locked) {
			camY += -Events::deltaY / Window::height * 2;
			camX += -Events::deltaX / Window::width * 2;

			float delta = Window::delta;

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

		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)Window::width / (float)Window::height, 0.1f, 100.0f);
		glm::mat4 view = camera->view();
		ourShader->uniform_matrix("projection", projection);
		ourShader->uniform_matrix("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		ourShader->uniform_matrix("model", model);
		ourModel.Draw(*ourShader.get());


		//sh.prepare_data();
		//sh.draw();
		//bmp.draw();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		Window::swap_buffers();
		Events::poll_events();
	}
	//delete_cude();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Window::terminate();
	return 0;
}