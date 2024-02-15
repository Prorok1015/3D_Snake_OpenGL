#include "inp_keyboard_device.h"
#include "../windows/window_system.h"

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
void input::KeyboardDevice::on_key_action(int keycode, int scancode, int action, int mode)
{
    KEY_ACTION act = to_action(action);

    if (act == KEY_ACTION::NONE) {
        return;
    }

    KEYBOARD_BUTTONS keyboard_keycode = (KEYBOARD_BUTTONS)keycode;
    on_key_keyboard_action(keyboard_keycode, act, application::WindowSystem::now_time());

    onKeyStateChanged(keyboard_keycode, act);

    egLOG("input/keyboard", "Key {}: action {}", keycode, action);
}

void input::KeyboardDevice::on_key_keyboard_action(KEYBOARD_BUTTONS keycode, KEY_ACTION action, float time)
{
    Key& state = keys_[keycode];
    prev_keys_[keycode] = state;
    //state = { action, time };
    state.action = action;
    state.time_stamp_down = action == KEY_ACTION::DOWN ? time : state.time_stamp_down;
    state.time_stamp_up = action == KEY_ACTION::UP ? time : state.time_stamp_up;
}

const inp::Key& input::KeyboardDevice::get_some_key(KEYBOARD_BUTTONS keycode, const KeyContainer& keys)
{
    auto it = keys.find(keycode);
    if (it != keys.end())
        return it->second;
    return {};
}
