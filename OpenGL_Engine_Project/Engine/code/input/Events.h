#pragma once

#include <array>
using uint = unsigned int;

class Events
{
public:
	static std::array<bool, 1032> _keys;
	static std::array<uint, 1032> _frames;
	static uint _current;
	static float deltaX;
	static float deltaY;
	static float x;
	static float y;
	static bool _cursor_locked;
	static bool _cursor_started;

	static int initialize();
	static void poll_events();

	static bool pressed(int keycode);
	static bool jpressed(int keycode);

	static bool clicked(int button);
	static bool jclicked(int button);

	static void toogle_cursor();
};

