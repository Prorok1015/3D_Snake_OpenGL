#pragma once
#include "../common/common.h"
#include "window.h"
#include <rnd_render_contex_interface.h>

namespace windows
{
	class WindowSystem
	{
	public:
		WindowSystem();
		~WindowSystem();
		WindowSystem(WindowSystem&&) = default;
		WindowSystem& operator= (WindowSystem&&) = default;

		WindowSystem(const WindowSystem&) = delete;
		WindowSystem& operator= (const WindowSystem&) = delete;

		std::shared_ptr<Window> make_window(std::string_view title, int width, int height);
		std::weak_ptr<Window> find_window(Window::Id win);
		bool is_all_windows_close();
		render::driver::render_context_interface* get_context() const { return context.get(); }
	private:
		std::unordered_map<Window::Id, std::weak_ptr<Window>, Window::Id::Hasher> _windows;
		std::unique_ptr<render::driver::render_context_interface> context = nullptr;
	};
	
	WindowSystem& get_system();
}

namespace wnd = windows;