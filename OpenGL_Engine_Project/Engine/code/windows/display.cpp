#include "display.h"
#include "../common/ds_store.hpp"
#include "../input/inp_input_system.h"
#include "window_system.h"

void application::Display::initialize(std::string_view title, int width, int height)
{
	WindowSystem* wndCreator = ds::DataStorage::instance().require<WindowSystem>();
	inp::InputSystem* inputSys = ds::DataStorage::instance().require<inp::InputSystem>();

	window = wndCreator->make_window(title, width, height);//
	input = std::make_shared<inp::InputManager>();
}
