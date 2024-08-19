#pragma once
#ifndef WINDOW_CAMERA_H_
#define WINDOW_CAMERA_H_

//#include <inp_input_manager.h>
#include <scn_transform_3d.hpp>
#include <common.h>

namespace input {
	class InputManager;
	using InputManagerRef = std::shared_ptr<InputManager>;
}

class camera_static {
public:
	camera_static(glm::vec3 position, glm::ivec2 size, float fov = glm::radians(45.0f));
	virtual ~camera_static() {};

	glm::mat4 projection() const;
	glm::mat4 view() const;

	virtual void enable_input_actions(input::InputManagerRef manager) {};
	virtual void disable_input_actions(input::InputManagerRef manager) {};

	void on_viewport_size_change(glm::ivec2 size);

	void set_enabled(bool enable) { enabled_ = enable; }
	bool is_enabled() const { return enabled_; }

	void move_to(glm::vec3 pos) {
		transform.move_to(pos);
	}

	void look_at(glm::vec3 at) {
		transform.look_at(at);
	}

	glm::ivec4 get_viewport() const { return glm::ivec4{ glm::ivec2{0,0}, window_size }; }

public:
	scn::Transform transform;

	float y = 0.f;
	glm::ivec2 window_size{ 0 };

	float fov_ = glm::radians(90.f);
	float distance = 100.f;
	float aspect = 0.f;

	bool enabled_ = false;
};

#endif /* WINDOW_CAMERA_H_ */


