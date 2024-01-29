#pragma once
#include "../common/common.h"
struct GLFWwindow;
enum class CursorMode;

//
//class Window
//{
//public:	
//	static int init_libraris();
//
//	static GLFWwindow* window;
//	static int width;
//	static int height;
//	static float lastTime;
//	static float delta;
//	static unsigned current_frame;
//
//	static int initialize(const char* title, int width, int height);
//	static void terminate();
//	static void swap_buffers();
//	static bool is_should_close();
//	static void set_should_close(bool close = true);
//	static void set_cursor_mode(CursorMode mode);
//
//	static float current_time();
//	static void update_frame();
//};

namespace application
{
	class Window
	{
	public:
		int initialize(const char* title, int width, int height);
		bool is_should_close();
		void swap_buffers();
		void set_should_close(bool close = true);
		void set_cursor_mode(CursorMode mode);
		void update_frame();

		static int init_libraris();
		static void terminate();
		static float current_time();
	private:
	public:
		GLFWwindow* window;
		int width;
		int height;
		float lastTime;
		float delta;
		unsigned current_frame;

		Event<std::function<void(GLFWwindow* window, double xpos, double ypos)>> cursor_position_window_event;
		Event<std::function<void(GLFWwindow* window, int button, int action, int mode)>> mouse_button_window_event;
		Event<std::function<void(GLFWwindow* window, int keycode, int scancode, int action, int mode)>> keyboard_window_event;
	};
}