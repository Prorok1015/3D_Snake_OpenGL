#include "inp_input_actions.h"
#include "../windows/window.h"

void input::InputActionMouseMove::update(float dt)
{
	InputSystem& inpSys = inp::get_system();
	glm::vec2 cur_pos = inpSys.mouse.get_pos();

	if (position != cur_pos) {
		onAction(cur_pos, position);
		position = cur_pos;
	}
}
