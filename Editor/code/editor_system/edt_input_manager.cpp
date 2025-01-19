#include "edt_input_manager.h"

namespace {
    struct event_visitor
    {
        bool operator() (const inp::keyboard_event& evt) const noexcept {
            return evt.action == inp::KEY_ACTION::DOWN;
        }
        bool operator() (const inp::mouse_click_event& evt) const noexcept {
            return evt.action == inp::KEY_ACTION::DOWN;
        }
        bool operator() (const inp::scroll_move_event& evt) const noexcept {
            return true;
        }

        bool operator() (const auto&) const noexcept {
            return false;
        }
    };
}

bool edt::input_manager::on_handle_event(const inp::input_event& evt)
{
    if (!is_block_keyaction_down || !is_block_keyaction_down_once) {
        return false;
    }

    return std::visit(event_visitor(), evt);
}

void edt::input_manager::on_notify_listeners(float dt)
{
    is_block_keyaction_down_once = true;

    notify_listeners(dt);
}

void edt::input_manager::unblock_layer_once()
{
    is_block_keyaction_down_once = false;
}

void edt::input_manager::block_layer()
{
    is_block_keyaction_down = true;
}

void edt::input_manager::unblock_layer()
{
    is_block_keyaction_down = false;
}
