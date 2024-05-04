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
	if (!is_enabled()) {
		return;
	}

	glm::vec2 delta = glm::vec3(pos - prev, 0);
	delta = -delta / (glm::vec2)window_size * 2.f;

	transform.add_rotate(delta.y, delta.x);
	transform.set_pitch(std::clamp(transform.get_pitch(), -glm::radians(89.0f), glm::radians(89.0f)));
}

void snakeengine::WASDCamera::wasd_move(inp::KEYBOARD_BUTTONS key, float dt)
{
	if (!is_enabled()) {
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
