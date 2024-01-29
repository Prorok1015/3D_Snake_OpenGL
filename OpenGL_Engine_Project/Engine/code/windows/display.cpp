#include "display.h"

void application::Display::initialize(const char* title, int width, int height)
{
	window = std::make_shared<Window>();
	input = std::make_shared<Input>();

	window->initialize(title, width, height);
	input->initialize(window);
	window->cursor_position_window_event += [this](auto, auto a, auto b) {
		input->cursor_position(a, b);
	};

	window->mouse_button_window_event += [this](auto, auto a, auto b, auto c) {
		input->mouse_button_callback(a, b, c);
	};

	window->keyboard_window_event += [this](auto, auto a, auto b, auto c, auto d) {
		input->key_callback(a, b, c, d);
	};

}
