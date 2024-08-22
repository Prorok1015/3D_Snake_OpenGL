#include "inp_mouse_device.h"

namespace {
	inp::KEY_ACTION to_action(int action)
	{
		switch (action)
		{
		case GLFW_RELEASE: return inp::KEY_ACTION::UP;
		case GLFW_PRESS: return inp::KEY_ACTION::DOWN;
			//case GLFW_REPEAT:;
		}
		return inp::KEY_ACTION::NONE;
	}
}

void inp::MouseDevice::on_mouse_move(double xpos, double ypos)
{
	prev_position = position;
	position = { xpos, ypos };
	//egLOG("", "Mouse pos x:{}, y:{}; prev x:{}, y:{}", position.x, position.y, prev_position.x, prev_position.y);
}

void inp::MouseDevice::on_mouse_scroll(double xpos, double ypos)
{
	scroll_direction = { xpos, ypos };
	//egLOG("", "Mouse pos x:{}, y:{}; prev x:{}, y:{}", position.x, position.y, prev_position.x, prev_position.y);
}

void inp::MouseDevice::on_mouse_button_action(int button, int action, int mode)
{
    KEY_ACTION act = to_action(action);

    if (act == KEY_ACTION::NONE) {
        return;
    }

    keys_[button] = act;
}

inp::Key inp::MouseDevice::get_key(MOUSE_BUTTONS key) const
{
	KEY_ACTION action = keys_[std::size_t(key)];
	return { action };
}
