#include "inp_keyboard_device.h"
#include "../windows/window_system.h"

void input::KeyboardDevice::on_key_action(int keycode, int scancode, int action, int mode)
{
    KEY_ACTION act = to_action(action);

    if (act == KEY_ACTION::NONE) {
        return;
    }

    KEYBOARD_BUTTONS keyboard_keycode = (KEYBOARD_BUTTONS)keycode;
    on_key_keyboard_action(keyboard_keycode, act, application::WindowSystem::now_time());

    //Key& key = keys_[keyboard_keycode];
    //key.action = act;

    //if (act == KEY_ACTION::DOWN) {
    //    key.time_stamp_down = application::WindowSystem::now_time();
    //    key.time_stamp_up = 0.f;
    //}
    //else if (act == KEY_ACTION::UP) {
    //    key.time_stamp_up = application::WindowSystem::now_time();
    //}

    //onKeyAction(keyboard_keycode, key);

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

inp::KEY_ACTION input::KeyboardDevice::to_action(int action)
{
    switch (action)
    {
    case GLFW_RELEASE: return inp::KEY_ACTION::UP;
    case GLFW_PRESS: return inp::KEY_ACTION::DOWN;
    //case GLFW_REPEAT:;
    }
    return KEY_ACTION::NONE;
}

const inp::Key& input::KeyboardDevice::get_some_key(KEYBOARD_BUTTONS keycode, const KeyContainer& keys)
{
    auto it = keys.find(keycode);
    if (it != keys.end())
        return it->second;
    return {};
}
