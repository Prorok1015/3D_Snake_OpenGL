#pragma once
#include "../common/common.h"
#include "inp_input_actions.h"

namespace input
{
	class InputManager
	{
	public:
		void poll_listeners(float dt);

		void registrate(std::shared_ptr<inp::InputActionBase> command) { commands_.push_back(command); };
		void unregistrate(std::shared_ptr<inp::InputActionBase> command) { commands_.erase(std::remove(commands_.begin(), commands_.end(), command)); };

		std::vector<std::shared_ptr<inp::InputActionBase>> commands_;
	};
}

namespace inp = input;