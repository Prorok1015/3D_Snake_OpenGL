#include "inp_input_manager.h"

void inp::input_manager::notify_listeners(float dt)
{
    if (!get_is_enabled()) {
        return;
    }

    auto& commands = get_active_commands();
    std::for_each(commands.begin(), commands.end(), [dt](auto cmd) { cmd->update(dt); });
}

void inp::input_manager::unregistrate(std::shared_ptr<inp::InputActionBase> command)
{
    auto& commands = get_active_commands();
    commands.erase(std::remove(commands.begin(), commands.end(), command));
};
