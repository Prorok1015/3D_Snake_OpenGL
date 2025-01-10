#pragma once
#include <common.h>
#include "inp_key_enums.hpp"
#include <glm/glm.hpp>

namespace inp
{

	class MouseDevice
	{
	public:
		glm::vec2 get_delta_pos() const { return prev_position - position; }
		const glm::vec2& get_pos() const { return position; }
		const glm::vec2& get_old_pos() const { return prev_position; }
		const glm::vec2& get_scroll() const { return scroll_direction; }

		void on_mouse_move(double xpos, double ypos);
		void on_mouse_button_action(int button, int action, int mode);
		void on_mouse_scroll(double xpos, double ypos);

		Key get_key(MOUSE_BUTTONS key) const;

		void clear_scroll() { scroll_direction = glm::zero<glm::vec2>(); }

		auto operator<=>(const MouseDevice&) const noexcept = default;

	private:
		glm::vec2 scroll_direction{ 0 };
		glm::vec2 position{ 0 };
		glm::vec2 prev_position{ 0 };
		std::array<KEY_ACTION, MOUSE_BUTTONS_COUNT> keys_;
	};
}
