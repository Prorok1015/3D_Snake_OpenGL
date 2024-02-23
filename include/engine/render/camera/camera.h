#pragma once
#ifndef WINDOW_CAMERA_H_
#define WINDOW_CAMERA_H_

#include <glm/glm.hpp>
#include "../../windows/window.h"
#include "../../input/inp_input_manager.h"

class Camera {
public:
	Camera(std::shared_ptr<inp::InputManager> inp_manager, glm::vec3 position, float fov);

	void attath_to_window(std::shared_ptr<wnd::Window> wnd);

	void rotate(float x, float y, float z);
	glm::mat4 projection(float fov);
	glm::mat4 view();

	void update();
	void mouse_move(glm::vec2, glm::vec2);

	void set_enabled(bool enable) { enabled_ = enable; }
	bool is_enabled() const { return enabled_; }

private:
	void update_vectors();
	void wasd_move(inp::KEYBOARD_BUTTONS key);

public:
	std::shared_ptr<inp::InputManager> input_;
	std::shared_ptr<wnd::Window> window_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;

	glm::vec3 position_;
	glm::mat4 rotation_;
	float fov_ = glm::radians(90.f);
	float speed_ = 15;

	float camX = 0.f;
	float camY = 0.f;

	bool enabled_ = false;
};

#endif /* WINDOW_CAMERA_H_ */


