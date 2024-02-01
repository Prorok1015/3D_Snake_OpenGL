#include "display.h"
#include "../common/ds_store.hpp"
#include "window_system.h"

void application::Display::initialize(const char* title, int width, int height)
{
	auto* wndCreator = ds::DataStorage::instance().require<WindowSystem>();

	window = wndCreator->make_window(title, width, height);//
	input = std::make_shared<Input>();
	input->initialize(window);
	window->eventMouseMove += [this](auto, auto a, auto b) {
		input->cursor_position(a, b);
	};

	window->eventMouseAction += [this](auto, auto a, auto b, auto c) {
		input->mouse_button_callback(a, b, c);
	};

	window->eventKeyboardAction += [this](auto, auto a, auto b, auto c, auto d) {
		input->key_callback(a, b, c, d);
	};

}
