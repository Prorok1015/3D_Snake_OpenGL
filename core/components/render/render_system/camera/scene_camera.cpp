#include "scene_camera.h"

void snakeengine::WASDCamera::enable_input_actions(inp::InputManagerRef manager)
{
	manager->create_hold_action(inp::KEYBOARD_BUTTONS::W, std::bind(&WASDCamera::wasd_move, this, inp::KEYBOARD_BUTTONS::W, std::placeholders::_1));
	manager->create_hold_action(inp::KEYBOARD_BUTTONS::A, std::bind(&WASDCamera::wasd_move, this, inp::KEYBOARD_BUTTONS::A, std::placeholders::_1));
	manager->create_hold_action(inp::KEYBOARD_BUTTONS::S, std::bind(&WASDCamera::wasd_move, this, inp::KEYBOARD_BUTTONS::S, std::placeholders::_1));
	manager->create_hold_action(inp::KEYBOARD_BUTTONS::D, std::bind(&WASDCamera::wasd_move, this, inp::KEYBOARD_BUTTONS::D, std::placeholders::_1));

	manager->create_mouse_move_action(std::bind(&WASDCamera::mouse_move, this, std::placeholders::_1, std::placeholders::_2));
}

void snakeengine::WASDCamera::disable_input_actions(inp::InputManagerRef manager)
{
}

void snakeengine::WASDCamera::mouse_move(glm::vec2 pos, glm::vec2 prev)
{
	if (!get_is_enabled()) {
		return;
	}

	glm::vec2 delta = glm::vec3(pos - prev, 0);
	delta = -delta / (glm::vec2)window_size * 2.f;

	transform.add_rotate(delta.y, delta.x);
	transform.set_pitch(std::clamp(transform.get_pitch(), -glm::radians(89.0f), glm::radians(89.0f)));
}

void snakeengine::WASDCamera::wasd_move(inp::KEYBOARD_BUTTONS key, float dt)
{
	if (!get_is_enabled()) {
		return;
	}


	switch (key)
	{
	case inp::KEYBOARD_BUTTONS::W:
		transform.add_position(transform.forward() * dt * move_speed());
		break;
	case inp::KEYBOARD_BUTTONS::A:
		transform.add_position(transform.left() * dt * move_speed());
		break;
	case inp::KEYBOARD_BUTTONS::S:
		transform.add_position(transform.back() * dt * move_speed());
		break;
	case inp::KEYBOARD_BUTTONS::D:
		transform.add_position(transform.right() * dt * move_speed());
		break;
	}
}

void snakeengine::MouseCamera::enable_input_actions(inp::InputManagerRef manager)
{
	manager->create_action(inp::MOUSE_BUTTONS::RIGHT, 
		std::bind(&MouseCamera::on_mouse_right, this, std::placeholders::_1, std::placeholders::_2));
	manager->create_action(inp::MOUSE_BUTTONS::LEFT, 
		std::bind(&MouseCamera::on_mouse_middle, this, std::placeholders::_1, std::placeholders::_2));
	manager->create_mouse_move_action(std::bind(&MouseCamera::on_mouse_move, this, std::placeholders::_1, std::placeholders::_2));
	manager->create_mouse_scroll_action(std::bind(&MouseCamera::on_mouse_whell, this, std::placeholders::_1, std::placeholders::_2));
}

void snakeengine::MouseCamera::on_mouse_middle(float dt, inp::KEY_ACTION act)
{
	if (act == inp::KEY_ACTION::DOWN) {
		isMove = true;
	}
	else if (act == inp::KEY_ACTION::UP) {
		isMove = false;
	}
}

void snakeengine::MouseCamera::on_mouse_right(float dt, inp::KEY_ACTION act)
{
	if (act == inp::KEY_ACTION::DOWN) {
		isLookAt = true;
	}
	else if (act == inp::KEY_ACTION::UP) {
		isLookAt = false;
	}
}

void snakeengine::MouseCamera::on_mouse_move(glm::vec2 cur, glm::vec2 prev)
{
	glm::vec2 delta = cur - prev;
	delta = -delta / glm::vec2(window_size) * 2.f;

	if (isLookAt) {
		world.add_rotate(delta.y, delta.x);
		world.set_pitch(std::clamp(world.get_pitch(), -glm::radians(90.0f), glm::radians(90.0f)));
	}

	if (isMove) {
		glm::vec3 addition = glm::vec3(delta.x, 0, delta.y);
		world.add_position(glm::rotateY(addition, world.get_yaw()) * get_move_speed());
	}
}


glm::mat4 snakeengine::MouseCamera::get_world_matrix() const
{
	return world.to_matrix() * transform.to_matrix();
}

glm::mat4 snakeengine::MouseCamera::view() const
{
	return inverse(get_world_matrix());
}

void snakeengine::MouseCamera::on_mouse_whell(glm::vec2 cur, glm::vec2 prev)
{
	distance -= cur.y * 3;
	distance = std::clamp(distance, 0.f, visible_distance);
	transform.set_pos(glm::vec3(0, 0, distance));
}
