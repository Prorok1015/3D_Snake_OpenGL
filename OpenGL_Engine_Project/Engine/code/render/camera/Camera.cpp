#include "camera.h"
#include <glm/ext.hpp>

Camera::Camera(application::Display& dis, glm::vec3 pos, float fov) : display(dis), position_(pos), fov_(fov), rotation_(1.0f) {
	update_vectors();
	display.input->create_hold_action(inp::KEYBOARD_BUTTONS::W, [this] { position_ += front_ * display.window->delta * speed_; });
	display.input->create_hold_action(inp::KEYBOARD_BUTTONS::A, [this] { position_ -= right_ * display.window->delta * speed_; });
	display.input->create_hold_action(inp::KEYBOARD_BUTTONS::S, [this] { position_ -= front_ * display.window->delta * speed_; });
	display.input->create_hold_action(inp::KEYBOARD_BUTTONS::D, [this] { position_ += right_ * display.window->delta * speed_; });

	display.input->create_mouse_move_action([this](glm::vec2 c, glm::vec2 p) {	mouse_move(c, p); });
}

void Camera::update_vectors() {
	front_ = glm::vec3(rotation_ * glm::vec4(0, 0, -1, 1));
	right_ = glm::vec3(rotation_ * glm::vec4(1, 0, 0, 1));
	up_ = glm::vec3(rotation_ * glm::vec4(0, 1, 0, 1));
}

void Camera::rotate(float x, float y, float z) {
	rotation_ = glm::rotate(rotation_, z, glm::vec3(0, 0, 1));
	rotation_ = glm::rotate(rotation_, y, glm::vec3(0, 1, 0));
	rotation_ = glm::rotate(rotation_, x, glm::vec3(1, 0, 0));

	update_vectors();
}

glm::mat4 Camera::projection(float fov) {
	float aspect = display.window->get_aspect_ratio();
	return glm::perspective(fov, aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::view() {
	return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::update()
{
	if (is_enabled()) {
		rotation_ = glm::mat4(1.0f);
		rotate(camY, camX, 0);
	}
}

void Camera::mouse_move(glm::vec2 pos, glm::vec2 prev)
{
	if (!is_enabled()) {
		return;
	}

	glm::vec2 delta = pos - prev;
	camY += -delta.y / display.window->height_ * 2;
	camX += -delta.x / display.window->width_ * 2;
	camY = std::clamp(camY, -glm::radians(89.0f), glm::radians(89.0f));
}
