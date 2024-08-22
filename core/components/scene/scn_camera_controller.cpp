#include "scn_camera_controller.h"
#include <wnd_window_system.h>

scn::mouse_camera_controller::mouse_camera_controller(rnd::camera* camera_)
	: camera(camera_)
{
	anchor.set_pitch(-glm::radians(45.0f));
	//anchor.set_pos(position);
	calculate_world_matrix();
}

void scn::mouse_camera_controller::enable_input_actions(inp::InputManagerRef manager)
{
	on_is_rotate_handler = manager->create_action(inp::MOUSE_BUTTONS::RIGHT,
		std::bind(&mouse_camera_controller::on_is_rotate, this, std::placeholders::_1, std::placeholders::_2));
	on_is_mouse_handler = manager->create_action(inp::MOUSE_BUTTONS::LEFT,
		std::bind(&mouse_camera_controller::on_is_move, this, std::placeholders::_1, std::placeholders::_2));
	on_mouse_move_handler = manager->create_mouse_move_action(
		std::bind(&mouse_camera_controller::on_mouse_move, this, std::placeholders::_1, std::placeholders::_2));
	on_mouse_whell_handler = manager->create_mouse_scroll_action(
		std::bind(&mouse_camera_controller::on_mouse_whell, this, std::placeholders::_1, std::placeholders::_2));

}

void scn::mouse_camera_controller::disable_input_actions(inp::InputManagerRef manager)
{
	manager->unregistrate(on_is_rotate_handler);
	manager->unregistrate(on_is_mouse_handler);
	manager->unregistrate(on_mouse_move_handler);
	manager->unregistrate(on_mouse_whell_handler);
}

void scn::mouse_camera_controller::on_is_move(float dt, inp::KEY_ACTION act)
{
	is_move = act == inp::KEY_ACTION::DOWN || act == inp::KEY_ACTION::NONE;
}

void scn::mouse_camera_controller::on_is_rotate(float dt, inp::KEY_ACTION act)
{
	is_rotate = act == inp::KEY_ACTION::DOWN || act == inp::KEY_ACTION::NONE;
}

void scn::mouse_camera_controller::on_mouse_move(glm::vec2 cur, glm::vec2 prev)
{
	glm::vec2 delta = cur - prev;
	delta = -delta / glm::vec2(camera->viewport_size) * 2.f;
	
	if (get_is_rotate()) {
		float pitch = delta.y;
		float yaw = delta.x;

		anchor.set_yaw(anchor.get_yaw() + yaw);
		anchor.set_pitch(std::clamp(anchor.get_pitch() + pitch, -glm::radians(90.0f), glm::radians(90.0f)));
	}

	if (get_is_move()) {
		glm::vec3 addition = glm::vec3(delta.x, 0, delta.y);
		glm::vec3 new_position = anchor.get_pos() + (glm::rotateY(addition, anchor.get_yaw()) * get_movement_speed());
		anchor.set_pos(new_position);
	}

	if (get_is_move() || get_is_rotate()) {
		calculate_world_matrix();
	}
}

void scn::mouse_camera_controller::on_mouse_whell(glm::vec2 cur, glm::vec2)
{
	distance -= cur.y * 3;
	distance = std::clamp(distance, 0.f, 150.f);
	calculate_world_matrix();
}

void scn::mouse_camera_controller::calculate_world_matrix()
{
	camera->world = anchor.to_matrix() * glm::translate(glm::mat4(1.0), glm::vec3(0, 0, distance));
}
