#include "scn_camera_controller.h"
#include <wnd_window_system.h>
#include <ecs/ecs_common_system.h>

scn::mouse_camera_controller::mouse_camera_controller(rnd::camera* camera_)
	: camera(camera_)
{
	rotation.x = -glm::radians(45.0f);
	anchor.set_euler_angles(rotation);
	ecs_connected_entity = camera_->ecs_entity;
	calculate_world_matrix();
}

void scn::mouse_camera_controller::enable_input_actions(inp::InputManagerRef manager)
{
	if (is_input_enabled) {
		return;
	}

	is_input_enabled = true;

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
	if (!is_input_enabled) {
		return;
	}

	is_input_enabled = false;

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

glm::quat remove_roll(const glm::quat& q) {
	glm::vec3 f = q * glm::vec3(0, 0, 1);
	glm::quat correction = glm::rotation(f, glm::vec3(0, 0, 1));
	return q * correction;
}

void scn::mouse_camera_controller::on_mouse_move(glm::vec2 cur, glm::vec2 prev)
{
	glm::vec2 delta = cur - prev;
	// translate to viewport coordinate
	delta = -delta / glm::vec2(camera->viewport_size) * 2.f;
	
	if (get_is_rotate()) {
		float pitch = delta.y;
		float yaw = delta.x;

		this->rotation.x = std::clamp(this->rotation.x + pitch, -glm::radians(90.0f), glm::radians(90.0f));
		this->rotation.y += yaw;
		
		anchor.set_euler_angles(rotation);
	}

	if (get_is_move()) {
		glm::vec3 addition = glm::vec3(delta.x, 0, delta.y);
		glm::vec3 new_position = anchor.get_pos() + (glm::rotateY(addition, rotation.y) * get_movement_speed());
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
	glm::mat4 orientation = glm::toMat4(anchor.get_rotation());
	glm::mat4 world = glm::translate(anchor.get_pos()) * orientation * glm::translate(glm::mat4(1.0), glm::vec3(0, 0, distance));
	ecs::add_component(ecs_connected_entity, update_camera_matrix_component{ .world = world });
}

void scn::mouse_camera_controller::set_camera(rnd::camera* cam)
{
	if (cam) {
		ecs_connected_entity = cam->ecs_entity;
		camera = cam;
	}
}

void scn::ecs_process_update_camera_matrix()
{
	for (auto& ent : ecs::filter<scn::update_camera_matrix_component, rnd::camera_component>())
	{
		auto* camera = ecs::get_component<rnd::camera_component>(ent);
		auto* matr = ecs::get_component<scn::update_camera_matrix_component>(ent);
		camera->world = matr->world;

		ecs::remove_component<scn::update_camera_matrix_component>(ent);
	}
}