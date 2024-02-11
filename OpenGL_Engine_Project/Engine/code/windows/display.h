#pragma once
#include "../common/common.h"
#include "window.h"
#include "../input/events.h"

namespace application
{
	class Display
	{
	public:
		void initialize(std::string_view title, int width, int height);

		std::shared_ptr<Window> window;
		std::shared_ptr<Input> input;
	};
}