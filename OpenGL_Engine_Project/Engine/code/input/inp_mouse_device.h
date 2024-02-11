#pragma once
#include "../common/common.h"

namespace input
{
	enum class MOUSE_BUTTONS
	{
		LEFT = GLFW_MOUSE_BUTTON_LEFT,
		RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
		MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,

		BTN_1 = LEFT,
		BTN_2 = RIGHT,
		BTN_3 = MIDDLE,
		BTN_4 = GLFW_MOUSE_BUTTON_4,
		BTN_5 = GLFW_MOUSE_BUTTON_5,
		BTN_6 = GLFW_MOUSE_BUTTON_6,
		BTN_7 = GLFW_MOUSE_BUTTON_7,
		BTN_8 = GLFW_MOUSE_BUTTON_8,
		LAST = BTN_8
	};

	constexpr std::size_t MOUSE_BUTTONS_COUNT = (int)MOUSE_BUTTONS::LAST + 1;

	class MouseDevice
	{
	public:
		glm::vec2 get_delta_pos() const { return old_position_ - position_; }
		const glm::vec2& get_pos() const { return position_; }
		const glm::vec2& get_old_pos() const { return old_position_; }
		
		void on_mouse_move(double xpos, double ypos);
		void on_mouse_button_action(int button, int action, int mode);
		void poll_events();

	private:
		glm::vec2 position_;
		glm::vec2 old_position_;
		std::array<bool, MOUSE_BUTTONS_COUNT> keys_;
	};
}

namespace inp = input;