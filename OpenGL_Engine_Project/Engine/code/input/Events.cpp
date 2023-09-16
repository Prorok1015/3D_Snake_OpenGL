#include "events.h"
#include "../windows/window.h"
#include "../common/enums.h"
#include <string>

std::array<bool, 1032>  Events::_keys;
std::array<uint, 1032> Events::_frames;
uint Events::_current = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::x = 0.0f;
float Events::y = 0.0f;
bool Events::_cursor_locked = false;
bool Events::_cursor_started = false;

constexpr auto _MOUSE_BUTTONS = 1024;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Events::_cursor_locked)
    {
        Events::deltaX += xpos - Events::x;
        Events::deltaY += ypos - Events::y;
    }
    else
    {
        Events::_cursor_started = true;
    }
    Events::x = xpos;
    Events::y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Events::_keys[_MOUSE_BUTTONS + button] = true;
        Events::_frames[_MOUSE_BUTTONS + button] = Events::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Events::_keys[_MOUSE_BUTTONS + button] = false;
        Events::_frames[_MOUSE_BUTTONS + button] = Events::_current;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        Events::_keys[key] = true;
        Events::_frames[key] = Events::_current;
    }
    else if (action == GLFW_RELEASE)
    {
        Events::_keys[key] = false;
        Events::_frames[key] = Events::_current;
    }
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
}

int Events::initialize()
{
    GLFWwindow* window = Window::window;
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    return 0;
}

void Events::poll_events()
{
    _current++;
    deltaX = 0.0f;
    deltaY = 0.0f;
    glfwPollEvents();
}

bool Events::pressed(int keycode)
{
    if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
        return false;

    return _keys[keycode];
}

bool Events::jpressed(int keycode)
{
    if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
        return false;

    return _keys[keycode] && (_frames[keycode] == _current);
}

bool Events::clicked(int button)
{
    return _keys[_MOUSE_BUTTONS + button];
}

bool Events::jclicked(int button)
{
    return _keys[_MOUSE_BUTTONS + button] && (_frames[_MOUSE_BUTTONS + button] == _current);
}

void Events::toogle_cursor() {
    _cursor_locked = !_cursor_locked;
    Window::setCursorMode(_cursor_locked ? CursorMode::Disable : CursorMode::Normal);
}