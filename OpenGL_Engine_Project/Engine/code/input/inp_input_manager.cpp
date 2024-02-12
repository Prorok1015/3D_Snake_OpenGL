#include "inp_input_manager.h"

void input::InputManager::poll_listeners(float dt)
{
    std::for_each(commands_.begin(), commands_.end(), [dt](auto cmd) { cmd->update(dt); });
}
