#include "inp_input_system.h"

void input::InputSystem::end_frame()
{
	keyboard.visit_keys(onKeyAction);
}
