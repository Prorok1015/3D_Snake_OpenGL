#if 0
#pragma once
#include "rnd_camera.h"
#include <inp_input_manager.h>

namespace snakeengine
{
	class WASDCamera : public rnd::camera
	{
	public:
		WASDCamera(glm::vec3 position, glm::ivec2 size) : camera(position, size) {}

		virtual void enable_input_actions(inp::InputManagerRef manager);
		virtual void disable_input_actions(inp::InputManagerRef manager);

	private:
		void mouse_move(glm::vec2, glm::vec2);
		void wasd_move(inp::KEYBOARD_BUTTONS key, float dt);
		float move_speed() const { return speed_; }

	private:
		float speed_ = 15;
	};





	class MouseCamera : public rnd::camera 
	{
	public:
		MouseCamera(glm::vec3 position, glm::ivec2 size)
			: camera(glm::vec3(0), size)
		{
			transform.set_pos(glm::vec3(0, 0, distance));
			anchor.set_pitch(-glm::radians(45.0f));
			anchor.set_pos(position);
		}

		virtual void enable_input_actions(inp::InputManagerRef manager);
		virtual void disable_input_actions(inp::InputManagerRef manager) {};

		glm::mat4 get_world_matrix() const;

		virtual glm::mat4 view() const override;
		eng::transform3d anchor;
	private:
		void on_mouse_middle(float dt, inp::KEY_ACTION act); 
		void on_mouse_right(float dt, inp::KEY_ACTION act); 
		void on_mouse_move(glm::vec2, glm::vec2);
		void on_mouse_whell(glm::vec2, glm::vec2);
		float get_move_speed() const { return speed_; }

	private:
		float distance = 20;
		float speed_ = 15.f;
		bool isMove = false;
		bool isLookAt = false;
	};
}
#endif