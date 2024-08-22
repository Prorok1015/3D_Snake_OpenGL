#pragma once
#include <common.h>
#include <inp_input_manager.h>
#include <eng_transform_3d.hpp>
#include <camera/rnd_camera.h>

namespace scn
{
	class mouse_camera_controller
	{
	public:
		mouse_camera_controller(rnd::camera* camera);

		void enable_input_actions(inp::InputManagerRef manager);
		void disable_input_actions(inp::InputManagerRef manager);

	private:
		void on_is_move(float dt, inp::KEY_ACTION act);
		void on_is_rotate(float dt, inp::KEY_ACTION act);
		void on_mouse_move(glm::vec2, glm::vec2);
		void on_mouse_whell(glm::vec2, glm::vec2);
		float get_movement_speed() const { return movement_speed; }
		bool get_is_move() const { return is_move; }
		bool get_is_rotate() const { return is_rotate; }

		void calculate_world_matrix();

	private:
		rnd::camera* camera = nullptr;
		eng::transform3d anchor;
		float distance = 20;
		float movement_speed = 15.f;
		bool is_move = false;
		bool is_rotate = false;

		std::shared_ptr<inp::InputActionBase> on_is_mouse_handler;
		std::shared_ptr<inp::InputActionBase> on_is_rotate_handler;
		std::shared_ptr<inp::InputActionBase> on_mouse_move_handler;
		std::shared_ptr<inp::InputActionBase> on_mouse_whell_handler;
	};
}