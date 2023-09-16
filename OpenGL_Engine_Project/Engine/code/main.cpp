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

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

int main()
{
	Window::initialize(WIDTH, HEIGHT, "Window 3.0");
	Events::initialize();
	UiSystem::initialize();

	glClearColor(0.0f, 0.2f, 0.2f, 1);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Camera* camera = new Camera(vec3(0, 0, 20), radians(90.0f));
	float lastTime = (float)glfwGetTime();
	float delta = 0.0f;

	//float camX = 0.0f;
	//float camY = 0.0f;

	//float speed = 15;

	//mat4 model(1.0f);
	//model = translate(model, vec3(0.5f, 0, 0));

	Shape sh;
	sh.pos = { 0, 0, 0 };
	sh.size = { WIDTH/2, HEIGHT/2 };

	Bitmap bmp;
	bmp.size = { 512, 512 };
	bmp.load_texture("./res/block.png");

	int speedShape = 100;
	float moveX = 0;
	float moveY = 0;
	bool isClk = false;
	while (!Window::isShouldClose()) {
		float currentTime = (float)glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		if (Events::jclicked(0)) {
			moveX = Events::x;
			moveY = Events::y;
			if (isClk) {
				sh.color.r = 0.f;
				sh.color.g = 1.f;
			} else {
				sh.color.r = 1.f;
				sh.color.g = 0.f;
			}
			isClk = !isClk;
		}

		glm::fvec2 forw = { moveX - sh.pos.x, moveY - sh.pos.y };
		forw = glm::normalize(forw);

		if (sh.pos.x != moveX) {
			sh.pos.x += speedShape * delta * forw.x;
		}
		if (sh.pos.y != moveY) {
			sh.pos.y += speedShape * delta * forw.y;
		}

		if (sh.pos.x + sh.size.x > Window::width) {
			sh.pos.x = Window::width - sh.size.x;
		}
		if (sh.pos.y + sh.size.y > Window::height) {
			sh.pos.y = Window::height - sh.size.y;
		}

		sh.prepare_data();
		bmp.prepare_data();

		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true);
		}
		if (Events::jpressed(GLFW_KEY_TAB)) {
			//Events::toogle_cursor();
		}
		if (Events::_cursor_locked) {
			//camY += -Events::deltaY / Window::height * 2;
			//camX += -Events::deltaX / Window::height * 2;

			//if (camY < -radians(89.0f)) {
			//	camY = -radians(89.0f);
			//}
			//if (camY > radians(89.0f)) {
			//	camY = radians(89.0f);
			//}

			//camera->rotation = mat4(1.0f);
			//camera->rotate(camY, camX, 0);
			//std::cout << "{ " << camX << ", " << camY << " }\n";
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sh.draw();
		bmp.draw();

		Window::swap_buffers();
		Events::poll_events();
	}

	Window::terminate();
	return 0;
}