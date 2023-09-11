#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>

#include "Window.h"
#include "Events.h"
#include "Shader.h"
#include "Texture.h"

#include "Camera.h"
#include "Mesh.h"
#include "Shape.h"

#include "ui_system.h"
#include "enums.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
float vertices[] = {
	// x    y
   -0.01f,-0.01f,
	0.01f, 0.01f,

   -0.01f, 0.01f,
	0.01f,-0.01f,
};

int attrs[] = {
		2,  0 //null terminator
};
int main()
{
	Window::initialize(WIDTH, HEIGHT, "Window 3.0");
	Events::initialize();
	UiSystem::initialize();

	std::shared_ptr<Shader> shader_ui = Shader::load("./res/screen.glslv", "./res/screen.glslf");
	if (shader_ui == nullptr) {
		std::cerr << "failed to load shader" << std::endl;
		Window::terminate();
		return 1;
	}

	//std::shared_ptr<Shader> crosshairShader = Shader::load("./res/crosshair.glslv", "./res/crosshair.glslf");
	//if (crosshairShader == nullptr) {
	//	std::cerr << "failed to load crosshair shader" << std::endl;
	//	Window::terminate();
	//	return 1;
	//}

	//std::shared_ptr<Texture> texture = Texture::load("./res/block.png");
	//if (texture == nullptr) {
	//	std::cerr << "failed to load texture" << std::endl;
	//	Window::terminate();
	//	return 1;
	//}

	glClearColor(0.0f, 0.2f, 0.2f, 1);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mesh* crosshair = new Mesh(vertices, 4, attrs);
	//Camera* camera = new Camera(vec3(0, 0, 20), radians(90.0f));
	float lastTime = glfwGetTime();
	float delta = 0.0f;

	//float camX = 0.0f;
	//float camY = 0.0f;

	//float speed = 15;

	//mat4 model(1.0f);
	//model = translate(model, vec3(0.5f, 0, 0));

	Shape sh;
	sh.pos = { 0, 0, 0 };
	sh.size = { WIDTH/4, HEIGHT/4 };

	int speedShape = 100;
	float moveX = 0;
	float moveY = 0;
	while (!Window::isShouldClose()) {
		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		if (Events::jclicked(0)) {
			moveX = Events::x;
			moveY = Events::y;
		}

		glm::fvec2 forw;

		forw.x = moveX - sh.pos.x;
		forw.y = moveY - sh.pos.y;

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

		sh.Invalidate();
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
		//crosshairShader->use();
		//crosshair->draw(DrawMode::Line);

		shader_ui->use();
		UiSystem::render(sh);

		Window::swap_buffers();
		Events::poll_events();
	}

	Window::terminate();
	return 0;
}