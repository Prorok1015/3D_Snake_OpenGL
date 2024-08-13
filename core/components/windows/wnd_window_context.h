#pragma once
#include <common.h>

struct GLFWwindow;

namespace window
{
	class WindowContext
	{
	public:
		WindowContext() = default;
		WindowContext(GLFWwindow* w)
			: _window_id(w), _is_inited(true)
		{
			init_impl();
		};
		~WindowContext() = default;
		WindowContext(const WindowContext&) = delete;
		WindowContext(WindowContext&&) = default;
		WindowContext& operator= (const WindowContext&) = delete;
		WindowContext& operator= (WindowContext&&) = default;

		bool is_inited() const { return _is_inited; }
		bool init(GLFWwindow* w);
		void swap_buffers();

	protected:
		void set_next_frame() { _frame += 1; }
		void init_impl();

		void set_window_id(GLFWwindow* w) { _window_id = w; }
		void set_is_inited(bool v) { _is_inited = v; }

	private:
		std::uint64_t _frame = 0;
		GLFWwindow* _window_id = nullptr;
		bool _is_inited = false;
	};
}
