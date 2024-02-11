#include "events.h"
#include "../windows/window.h"
#include "../common/enums.h"
#include "../game_system/game_system.h"
#include "../common/ds_store.hpp"
#include <string>

using namespace application;

void application::Input::poll_events()
{
    deltaX = 0.0f;
    deltaY = 0.0f;
    ++current_frame;
    glfwPollEvents();
}

void application::Input::toogle_cursor()
{
    auto game = ds::DataStorage::instance().require<app::GameSystem>();
    auto win = game->get_window();
    _cursor_locked = !_cursor_locked;
    win->set_cursor_mode(_cursor_locked ? CursorMode::Disable : CursorMode::Normal);
}

void application::Input::poll_listeners(float dt)
{
    std::for_each(commands_.begin(), commands_.end(), [dt](auto cmd) { cmd->update(dt); });
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
