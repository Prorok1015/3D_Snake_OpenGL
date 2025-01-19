#include "inp_input_manager.h"
#include "common.h"

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

bool inp::input_manager::on_handle_event(const input_event& evt)
{
    if (!is_block_keyaction_down || !is_block_keyaction_down_once) {
        return false;
    }

    return std::visit(event_visitor(), evt);
}

void inp::input_manager::notify_listeners(float dt)
{
    is_block_keyaction_down_once = true;

    if (!get_is_enabled()) {
        return;
    }

    auto& commands = get_active_commands();
    std::for_each(commands.begin(), commands.end(), [dt](auto cmd) { cmd->update(dt); });
}

void inp::input_manager::unblock_layer_once()
{
    is_block_keyaction_down_once = false;
}

void inp::input_manager::block_layer()
{
    is_block_keyaction_down = true;
}

void inp::input_manager::unblock_layer()
{
    is_block_keyaction_down = false;
}

void inp::input_manager::unregistrate(std::shared_ptr<inp::InputActionBase> command)
{
    auto& commands = get_active_commands();
    commands.erase(std::remove(commands.begin(), commands.end(), command));
};
