#pragma once
#include "../windows/window.h"

namespace debug_ui
{
	void initialize(std::shared_ptr<application::Window> win);
}

namespace dbg_ui = debug_ui;