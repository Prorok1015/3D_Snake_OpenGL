#pragma once
#include "../common/common.h"
#include "inp_keyboard_device.h"
#include "inp_input_actions.h"

enum class MOUSE
{
	BTN_1 = GLFW_MOUSE_BUTTON_1,
	BTN_2 = GLFW_MOUSE_BUTTON_2,
	BTN_3 = GLFW_MOUSE_BUTTON_3,
	BTN_4 = GLFW_MOUSE_BUTTON_4,
	BTN_5 = GLFW_MOUSE_BUTTON_5,
	BTN_6 = GLFW_MOUSE_BUTTON_6,
	BTN_7 = GLFW_MOUSE_BUTTON_7,
	BTN_8 = GLFW_MOUSE_BUTTON_8,
	LAST = GLFW_MOUSE_BUTTON_LAST,
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
};

namespace application
{
	class Input
	{
	public:
		void poll_events();
		void toogle_cursor();
		void poll_listeners(float dt);

		void cursor_position(double xpos, double ypos);

		void registrate(std::shared_ptr<inp::InputActionBase> command) { commands_.push_back(command); };
		void unregistrate(std::shared_ptr<inp::InputActionBase> command) { commands_.erase(std::remove(commands_.begin(), commands_.end(), command)); };

		float deltaX = 0.f;
		float deltaY = 0.f;
		float x = 0.f;
		float y = 0.f;
		bool _cursor_locked = false;
		bool _cursor_started = false;
		std::size_t current_frame = 0;
		std::vector<std::shared_ptr<inp::InputActionBase>> commands_;
	};
}

namespace app = application;