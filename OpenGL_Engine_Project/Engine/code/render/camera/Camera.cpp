#include "camera.h"
#include <glm/ext.hpp>

Camera::Camera(std::shared_ptr<inp::InputManager> inp_m, glm::vec3 pos, float fov) 
	: input_(inp_m), position_(pos), fov_(fov), rotation_(1.0f) 
{
	update_vectors();
}

void Camera::attath_to_window(std::shared_ptr<wnd::Window> wnd)
{
	if (wnd == nullptr) {
		return;
	}

	window_ = wnd;
	input_->create_hold_action(inp::KEYBOARD_BUTTONS::W, [this] { wasd_move(inp::KEYBOARD_BUTTONS::W); });
	input_->create_hold_action(inp::KEYBOARD_BUTTONS::A, [this] { wasd_move(inp::KEYBOARD_BUTTONS::A); });
	input_->create_hold_action(inp::KEYBOARD_BUTTONS::S, [this] { wasd_move(inp::KEYBOARD_BUTTONS::S); });
	input_->create_hold_action(inp::KEYBOARD_BUTTONS::D, [this] { wasd_move(inp::KEYBOARD_BUTTONS::D); });

	input_->create_mouse_move_action([this](glm::vec2 c, glm::vec2 p) {	mouse_move(c, p); });
}

void Camera::update_vectors() {
	front_ = glm::vec3(rotation_ * glm::vec4(0, 0, -1, 1));
	right_ = glm::vec3(rotation_ * glm::vec4(1, 0, 0, 1));
	up_ = glm::vec3(rotation_ * glm::vec4(0, 1, 0, 1));
}

void Camera::wasd_move(inp::KEYBOARD_BUTTONS key)
{
	if (!is_enabled()) {
		return;
	}

	switch (key)
	{
	case inp::KEYBOARD_BUTTONS::W: position_ += front_ * window_->delta * speed_; break;
	case inp::KEYBOARD_BUTTONS::A: position_ -= right_ * window_->delta * speed_; break;
	case inp::KEYBOARD_BUTTONS::S: position_ -= front_ * window_->delta * speed_; break;
	case inp::KEYBOARD_BUTTONS::D: position_ += right_ * window_->delta * speed_; break;
	}
}

void Camera::rotate(float x, float y, float z) {
	rotation_ = glm::rotate(rotation_, z, glm::vec3(0, 0, 1));
	rotation_ = glm::rotate(rotation_, y, glm::vec3(0, 1, 0));
	rotation_ = glm::rotate(rotation_, x, glm::vec3(1, 0, 0));

	update_vectors();
}

glm::mat4 Camera::projection(float fov) {
	float aspect = window_->get_aspect_ratio();
	return glm::perspective(fov_, aspect, 0.1f, 100.0f);
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
	camY += -delta.y / window_->height_ * 2;
	camX += -delta.x / window_->width_ * 2;
	camY = std::clamp(camY, -glm::radians(89.0f), glm::radians(89.0f));
}
