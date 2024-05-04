#pragma once
#ifndef WINDOW_CAMERA_H_
#define WINDOW_CAMERA_H_

#include "../../input/inp_input_manager.h"

#include "../../scene/scn_transform_3d.hpp"

class Camera {
public:
	Camera(glm::vec3 position, glm::ivec2 size, float fov = glm::radians(45.0f));
	virtual ~Camera() {};

	glm::mat4 projection() const;
	glm::mat4 view() const;

	virtual void enable_input_actions(inp::InputManagerRef manager) {};
	virtual void disable_input_actions(inp::InputManagerRef manager) {};

	void on_viewport_size_change(glm::ivec2 size);

	void set_enabled(bool enable) { enabled_ = enable; }
	bool is_enabled() const { return enabled_; }

	void move_to(glm::vec3 pos) {
		transform.move_to(pos);
	}

	void look_at(glm::vec3 at) {
		transform.look_at(at);
	}

public:
	scn::Transform transform;

	glm::ivec2 window_size{ 0 };

	float fov_ = glm::radians(90.f);
	float distance = 100.f;
	float aspect = 0.f;

	bool enabled_ = false;
};

#endif /* WINDOW_CAMERA_H_ */


