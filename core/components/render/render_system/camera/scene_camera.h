#pragma once
#include "camera.h"
#include <inp_input_manager.h>

namespace snakeengine
{
	class WASDCamera : public camera_static
	{
	public:
		WASDCamera(glm::vec3 position, glm::ivec2 size) : camera_static(position, size) {}

		virtual void enable_input_actions(inp::InputManagerRef manager);
		virtual void disable_input_actions(inp::InputManagerRef manager);

	private:
		void mouse_move(glm::vec2, glm::vec2);
		void wasd_move(inp::KEYBOARD_BUTTONS key, float dt);
		float move_speed() const { return speed_; }

	private:
		float speed_ = 15;
	};

	class MouseCamera : public camera_static 
	{
	public:
		MouseCamera(glm::vec3 position, glm::ivec2 size) : camera_static(position, size) {}

		virtual void enable_input_actions(inp::InputManagerRef manager);
		virtual void disable_input_actions(inp::InputManagerRef manager) {};
	private:
		void mouse_middle_key_down(float dt) { isMove = true; }
		void mouse_middle_key_up(float dt) { isMove = false; }
		void mouse_right_key_down(float dt);
		void mouse_right_key_up(float dt) { isLookAt = false; }
		void mouse_move(glm::vec2, glm::vec2);
		void mouse_whell();
		float move_speed() const { return speed_; }

	private:
		float speed_ = 1;
		bool isMove = false;
		bool isLookAt = false;
	};
}