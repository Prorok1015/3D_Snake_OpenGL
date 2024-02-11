#include "inp_mouse_device.h"

void input::MouseDevice::on_mouse_move(double xpos, double ypos)
{
	old_position_ = position_;
	position_ = { xpos, ypos };
}

void input::MouseDevice::on_mouse_button_action(int button, int action, int mode)
{
    if (action != GLFW_RELEASE && action != GLFW_PRESS) {
        return;
    }

    keys_[button] = action;
}
