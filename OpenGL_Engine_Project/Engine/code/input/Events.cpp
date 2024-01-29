#include "events.h"
#include "../windows/window.h"
#include "../common/enums.h"
#include <string>

using namespace application;
//std::unordered_map<KeyAction, Events::KeyStruct, KeyAction::Hasher> Events::kkeys;
//std::unordered_map<KeyAction, Event<std::function<void()>>, KeyAction::Hasher> Events::listeners;
//float Events::deltaX = 0.0f;
//float Events::deltaY = 0.0f;
//float Events::x = 0.0f;
//float Events::y = 0.0f;
//bool Events::_cursor_locked = false;
//bool Events::_cursor_started = false;
//
//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    if (Events::_cursor_locked)
//    {
//        Events::deltaX += xpos - Events::x;
//        Events::deltaY += ypos - Events::y;
//    } else {
//        Events::_cursor_started = true;
//    }
//    Events::x = xpos;
//    Events::y = ypos;
//}
//
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
//{
//    if (action != GLFW_RELEASE && action != GLFW_PRESS) {
//        return;
//    }
//
//    auto& key = Events::kkeys[(MOUSE)button];
//    key.status = action == GLFW_PRESS ? Events::KeyStruct::PRESS : Events::KeyStruct::RELEASE;
//    key.frame_action = Window::current_frame;
//}
//
//void key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mode)
//{
//    if (action != GLFW_RELEASE && action != GLFW_PRESS) {
//        return;
//    }
//
//    auto& key = Events::kkeys[(KEYBOARD)keycode];
//    key.status = action == GLFW_PRESS ? Events::KeyStruct::PRESS : Events::KeyStruct::RELEASE;
//    key.frame_action = Window::current_frame;
//}
//
//int Events::initialize()
//{
//    //GLFWwindow* window = Window::window;
//    //glfwSetKeyCallback(window, key_callback);
//    //glfwSetMouseButtonCallback(window, mouse_button_callback);
//    //glfwSetCursorPosCallback(window, cursor_position_callback);
//    return 0;
//}
//
//void Events::poll_events()
//{
//    deltaX = 0.0f;
//    deltaY = 0.0f;
//    glfwPollEvents();
//}
//
//bool Events::pressed(KEYBOARD keycode)
//{
//    return kkeys[keycode].status == Events::KeyStruct::PRESS;
//}
//
//bool Events::jpressed(KEYBOARD keycode)
//{
//    return kkeys[keycode].is_pressed() && (kkeys[keycode].frame_action == Window::current_frame);
//}
//
//bool Events::clicked(MOUSE button)
//{
//    return kkeys[button].is_pressed();
//}
//
//bool Events::jclicked(MOUSE button)
//{
//    return kkeys[button].is_pressed() &&
//        kkeys[button].frame_action == Window::current_frame;
//}
//
//void Events::toogle_cursor() {
//    _cursor_locked = !_cursor_locked;
//    Window::set_cursor_mode(_cursor_locked ? CursorMode::Disable : CursorMode::Normal);
//}
//
//void Events::poll_listeners()
//{
//    for (const auto& [key, callbacks] : listeners)
//    {
//        if (!kkeys[key].is_pressed()) {
//            continue;
//        }
//        for (const auto& cb : callbacks) {
//            cb();
//        }
//    }
//}

int application::Input::initialize(std::weak_ptr<Window> win)
{
	window = win;

	return 0;
}

void application::Input::poll_events()
{
    deltaX = 0.0f;
    deltaY = 0.0f;
    glfwPollEvents();
}

bool application::Input::pressed(KEYBOARD keycode)
{
	return kkeys[keycode].status == Input::KeyStruct::PRESS;
}

bool application::Input::jpressed(KEYBOARD keycode)
{
    if (auto win = window.lock()) {
        return kkeys[keycode].is_pressed() && (kkeys[keycode].frame_action == win->current_frame);
    }
	return false;
}

bool application::Input::clicked(MOUSE button)
{
	return kkeys[button].is_pressed();
}

bool application::Input::jclicked(MOUSE button)
{
    if (auto win = window.lock()) {
        return kkeys[button].is_pressed() &&
            kkeys[button].frame_action == win->current_frame;
    }
    return false;
}

void application::Input::toogle_cursor()
{
    if (auto win = window.lock()) {
        _cursor_locked = !_cursor_locked;
        win->set_cursor_mode(_cursor_locked ? CursorMode::Disable : CursorMode::Normal);
    }
}

void application::Input::poll_listeners()
{
	for (const auto& [key, callbacks] : listeners)
    {
        if (!kkeys[key].is_pressed()) {
            continue;
        }
        for (const auto& cb : callbacks) {
            cb();
        }
    }
}

void application::Input::cursor_position(double xpos, double ypos)
{
    if (_cursor_locked)
    {
        deltaX += xpos - x;
        deltaY += ypos - y;
    } else {
        _cursor_started = true;
    }
    x = xpos;
    y = ypos;
}

void application::Input::mouse_button_callback(int button, int action, int mode)
{
    if (action != GLFW_RELEASE && action != GLFW_PRESS) {
        return;
    }

    if (auto win = window.lock()) {
        auto& key = kkeys[(MOUSE)button];
        key.status = action == GLFW_PRESS ? KeyStruct::PRESS : KeyStruct::RELEASE;
        key.frame_action = win->current_frame;
    }
}

void application::Input::key_callback(int keycode, int scancode, int action, int mode)
{
    if (action != GLFW_RELEASE && action != GLFW_PRESS) {
        return;
    }
    if (auto win = window.lock()) {
        auto& key = kkeys[(KEYBOARD)keycode];
        key.status = action == GLFW_PRESS ? KeyStruct::PRESS : KeyStruct::RELEASE;
        key.frame_action = win->current_frame;
    }
}
