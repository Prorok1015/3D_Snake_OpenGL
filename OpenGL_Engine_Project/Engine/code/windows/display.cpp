#include "display.h"
#include "../common/ds_store.hpp"
#include "window_system.h"
#include "../input/inp_input_system.h"

void application::Display::initialize(std::string_view title, int width, int height)
{
	WindowSystem* wndCreator = ds::DataStorage::instance().require<WindowSystem>();
	inp::InputSystem* inputSys = ds::DataStorage::instance().require<inp::InputSystem>();

	window = wndCreator->make_window(title, width, height);//
	input = std::make_shared<Input>();

	//inputSys->onKeyAction += [this](auto a, auto b) { input->on_keyboard_key_action(a, b); };

	window->eventMouseMove += [this](auto, auto a, auto b) {
		input->cursor_position(a, b);
	};

	//window->eventMouseAction += [this](auto, auto a, auto b, auto c) {
	//	//input->mouse_button_callback(a, b, c);
	//};

	//window->eventKeyboardAction += [this](auto, auto a, auto b, auto c, auto d) {
	//	//input->key_callback(a, b, c, d);
	//	//keyboard->on_key_action(a, b, c, d);
	//};

}
