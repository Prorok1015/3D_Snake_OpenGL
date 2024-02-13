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

void input::MouseDevice::on_mouse_move(double xpos, double ypos)
{
	prev_position_ = position_;
	position_ = { xpos, ypos };
	//egLOG("", "Mouse pos x:{}, y:{}; prev x:{}, y:{}", position_.x, position_.y, prev_position_.x, prev_position_.y);
}

void input::MouseDevice::on_mouse_button_action(int button, int action, int mode)
{
    KEY_ACTION act = to_action(action);

    if (act == KEY_ACTION::NONE) {
        return;
    }

    keys_[button] = act;
}

const input::Key& input::MouseDevice::get_key(MOUSE_BUTTONS key) const
{
	KEY_ACTION action = keys_[std::size_t(key)];
	return { action };
}
