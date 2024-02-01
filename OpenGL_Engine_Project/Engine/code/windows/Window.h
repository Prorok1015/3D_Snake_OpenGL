#pragma once
#include "../common/common.h"
struct GLFWwindow;
enum class CursorMode;

namespace application
{
	class Window
	{
	public:
		Window(std::string_view title, int width, int height);
		bool is_should_close();
		void swap_buffers();
		void set_should_close(bool close = true);
		void set_cursor_mode(CursorMode mode);
		void update_frame();

		void on_resize_window(int width, int height);
		void on_mouse_move(double xpos, double ypos);
		void on_mouse_button_action(int button, int action, int mode);
		void on_keyboard_action(int keycode, int scancode, int action, int mode);

		static float current_time();
	private:
	public:
		GLFWwindow* window = nullptr;
		int width = 0;
		int height = 0;
		float delta = 0.f;
		float lastTime = 0.f;
		std::size_t current_frame = 0;

		Event<std::function<void(GLFWwindow* window, int width, int height)>> eventResizeWindow;
		Event<std::function<void(GLFWwindow* window, double xpos, double ypos)>> eventMouseMove;
		Event<std::function<void(GLFWwindow* window, int button, int action, int mode)>> eventMouseAction;
		Event<std::function<void(GLFWwindow* window, int keycode, int scancode, int action, int mode)>> eventKeyboardAction;
	};
}