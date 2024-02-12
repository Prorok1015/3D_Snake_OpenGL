#pragma once
#include "../common/common.h"
#include "../input/inp_input_manager.h"
#include "window.h"

namespace application
{
	class Display
	{
	public:
		void initialize(std::string_view title, int width, int height);

		std::shared_ptr<Window> window;
	//private:
		std::shared_ptr<inp::InputManager> input;
	};
}