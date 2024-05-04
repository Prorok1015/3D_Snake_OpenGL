#pragma once
#include "camera.h"

namespace snakeengine
{
	class WASDCamera : public Camera
	{
	public:
		WASDCamera(glm::vec3 position, glm::ivec2 size) : Camera(position, size) {}

		virtual void enable_input_actions(inp::InputManagerRef manager);
		virtual void disable_input_actions(inp::InputManagerRef manager);

	private:
		void mouse_move(glm::vec2, glm::vec2);
		void wasd_move(inp::KEYBOARD_BUTTONS key, float dt);
		float move_speed() const { return speed_; }

	private:
		float speed_ = 15;
	};
}