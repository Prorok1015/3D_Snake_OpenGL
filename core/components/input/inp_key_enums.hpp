#pragma once
#include <common.h>
//#include <GLFW/glfw3.h>

namespace inp
{
	enum class KEY_ACTION
	{
		NONE,
		UP,
		DOWN,
	};

	enum class KEYBOARD_BUTTONS
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH,
		K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,
		SEMICOLON, EQUAL,
		ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DELETE,
		RIGHT, LEFT, DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END,
		CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
		KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9, KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL,
		LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU
	};

	//enum class KEYBOARD_BUTTONS2
	//{
	//	A = GLFW_KEY_A,
	//	B = GLFW_KEY_B,
	//	C = GLFW_KEY_C,
	//	D = GLFW_KEY_D,
	//	E = GLFW_KEY_E,
	//	F = GLFW_KEY_F,
	//	G = GLFW_KEY_G,
	//	H = GLFW_KEY_H,
	//	I = GLFW_KEY_I,
	//	J = GLFW_KEY_J,
	//	K = GLFW_KEY_K,
	//	L = GLFW_KEY_L,
	//	M = GLFW_KEY_M,
	//	N = GLFW_KEY_N,
	//	O = GLFW_KEY_O,
	//	P = GLFW_KEY_P,
	//	Q = GLFW_KEY_Q,
	//	R = GLFW_KEY_R,
	//	S = GLFW_KEY_S,
	//	T = GLFW_KEY_T,
	//	U = GLFW_KEY_U,
	//	V = GLFW_KEY_V,
	//	W = GLFW_KEY_W,
	//	X = GLFW_KEY_X,
	//	Y = GLFW_KEY_Y,
	//	Z = GLFW_KEY_Z,
	//	SPACE = GLFW_KEY_SPACE,
	//	APOSTROPHE = GLFW_KEY_APOSTROPHE,/* ' */
	//	COMMA = GLFW_KEY_COMMA,			 /* , */
	//	MINUS = GLFW_KEY_MINUS,			 /* - */
	//	PERIOD = GLFW_KEY_PERIOD,		 /* . */
	//	SLASH = GLFW_KEY_SLASH,			 /* / */
	//	K_0 = GLFW_KEY_0,
	//	K_1 = GLFW_KEY_1,
	//	K_2 = GLFW_KEY_2,
	//	K_3 = GLFW_KEY_3,
	//	K_4 = GLFW_KEY_4,
	//	K_5 = GLFW_KEY_5,
	//	K_6 = GLFW_KEY_6,
	//	K_7 = GLFW_KEY_7,
	//	K_8 = GLFW_KEY_8,
	//	K_9 = GLFW_KEY_9,
	//	SEMICOLON = GLFW_KEY_SEMICOLON,     /* ; */
	//	EQUAL = GLFW_KEY_EQUAL,				/* = */
	//	ESCAPE = GLFW_KEY_ESCAPE,
	//	ENTER = GLFW_KEY_ENTER,
	//	TAB = GLFW_KEY_TAB,
	//	BACKSPACE = GLFW_KEY_BACKSPACE,
	//	INSERT = GLFW_KEY_INSERT,
	//	DELETE = GLFW_KEY_DELETE,
	//	RIGHT = GLFW_KEY_RIGHT,
	//	LEFT = GLFW_KEY_LEFT,
	//	DOWN = GLFW_KEY_DOWN,
	//	UP = GLFW_KEY_UP,
	//	PAGE_UP = GLFW_KEY_PAGE_UP,
	//	PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
	//	HOME = GLFW_KEY_HOME,
	//	END = GLFW_KEY_END,
	//	CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
	//	SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
	//	NUM_LOCK = GLFW_KEY_NUM_LOCK,
	//	PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
	//	PAUSE = GLFW_KEY_PAUSE,
	//	F1 = GLFW_KEY_F1,
	//	F2 = GLFW_KEY_F2,
	//	F3 = GLFW_KEY_F3,
	//	F4 = GLFW_KEY_F4,
	//	F5 = GLFW_KEY_F5,
	//	F6 = GLFW_KEY_F6,
	//	F7 = GLFW_KEY_F7,
	//	F8 = GLFW_KEY_F8,
	//	F9 = GLFW_KEY_F9,
	//	F10 = GLFW_KEY_F10,
	//	F11 = GLFW_KEY_F11,
	//	F12 = GLFW_KEY_F12,
	//	F13 = GLFW_KEY_F13,
	//	F14 = GLFW_KEY_F14,
	//	F15 = GLFW_KEY_F15,
	//	F16 = GLFW_KEY_F16,
	//	F17 = GLFW_KEY_F17,
	//	F18 = GLFW_KEY_F18,
	//	F19 = GLFW_KEY_F19,
	//	F20 = GLFW_KEY_F20,
	//	F21 = GLFW_KEY_F21,
	//	F22 = GLFW_KEY_F22,
	//	F23 = GLFW_KEY_F23,
	//	F24 = GLFW_KEY_F24,
	//	F25 = GLFW_KEY_F25,
	//	KP_0 = GLFW_KEY_KP_0,
	//	KP_1 = GLFW_KEY_KP_1,
	//	KP_2 = GLFW_KEY_KP_2,
	//	KP_3 = GLFW_KEY_KP_3,
	//	KP_4 = GLFW_KEY_KP_4,
	//	KP_5 = GLFW_KEY_KP_5,
	//	KP_6 = GLFW_KEY_KP_6,
	//	KP_7 = GLFW_KEY_KP_7,
	//	KP_8 = GLFW_KEY_KP_8,
	//	KP_9 = GLFW_KEY_KP_9,
	//	KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
	//	KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
	//	KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
	//	KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
	//	KP_ADD = GLFW_KEY_KP_ADD,
	//	KP_ENTER = GLFW_KEY_KP_ENTER,
	//	KP_EQUAL = GLFW_KEY_KP_EQUAL,
	//	LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
	//	LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
	//	LEFT_ALT = GLFW_KEY_LEFT_ALT,
	//	LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
	//	RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
	//	RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
	//	RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
	//	RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
	//	MENU = GLFW_KEY_MENU,
	//};

	enum class MOUSE_BUTTONS
	{
		LEFT,
		RIGHT,
		MIDDLE,

		BTN_1 = LEFT,
		BTN_2 = RIGHT,
		BTN_3 = MIDDLE,
		BTN_4,
		BTN_5,
		BTN_6,
		BTN_7,
		BTN_8,
		LAST,
		COUNT
	};

	constexpr std::size_t MOUSE_BUTTONS_COUNT = (std::size_t)MOUSE_BUTTONS::COUNT;

	struct Key
	{
		KEY_ACTION action = KEY_ACTION::NONE;
		float time_stamp_down = 0.f;
		float time_stamp_up = 0.f;
	};

}
 