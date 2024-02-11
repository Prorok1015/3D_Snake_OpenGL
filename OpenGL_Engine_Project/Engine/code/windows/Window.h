#pragma once
#include "../common/common.h"
struct GLFWwindow;
enum class CursorMode;

namespace application
{

	class Window
	{
	public:
		struct Id
		{
			GLFWwindow* id_ = nullptr;
			auto operator<=>(const Id&) const = default;
			std::size_t get_id() const { return std::size_t(id_); }
			operator GLFWwindow* () const { return id_; }
			struct hasher {
				std::size_t operator() (const Id& id) const { return id.get_id(); }
			};
		};

		Window(std::string_view title, int width, int height);
		~Window();
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
		Id id_;
		int width_ = 0;
		int height_ = 0;
		float delta = 0.f;
		float lastTime = 0.f;
		std::size_t current_frame = 0;

		Event<void(Window& window, int width, int height)> eventResizeWindow;
		Event<void(Window& window, double xpos, double ypos)> eventMouseMove;
		Event<void(Window& window, int button, int action, int mode)> eventMouseAction;
		Event<void(Window& window, int keycode, int scancode, int action, int mode)> eventKeyboardAction;
	};
}

namespace app = application;