#pragma once
#include <common.h>
#include "inp_key_enums.hpp"
#include <glm/glm.hpp>

namespace input
{

	class MouseDevice
	{
	public:
		glm::vec2 get_delta_pos() const { return prev_position_ - position_; }
		const glm::vec2& get_pos() const { return position_; }
		const glm::vec2& get_old_pos() const { return prev_position_; }
		
		void on_mouse_move(double xpos, double ypos);
		void on_mouse_button_action(int button, int action, int mode);

		Key get_key(MOUSE_BUTTONS key) const;

	private:
		glm::vec2 position_;
		glm::vec2 prev_position_;
		std::array<KEY_ACTION, MOUSE_BUTTONS_COUNT> keys_;
	};
}

namespace inp = input;