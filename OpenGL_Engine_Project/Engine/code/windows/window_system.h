#pragma once
#include "../common/common.h"
#include "window.h"

namespace application
{
	class WindowSystem
	{
	public:
		WindowSystem();
		~WindowSystem();
		WindowSystem(const WindowSystem&) = delete;
		WindowSystem(WindowSystem&&) = delete;
		WindowSystem& operator= (const WindowSystem&) = delete;
		WindowSystem& operator= (WindowSystem&&) = delete;

		std::shared_ptr<Window> make_window(std::string_view title, int width, int height);
		std::weak_ptr<Window> find_window(Window::Id win);
		bool is_all_windows_close();
		static float now_time();
	private:
		std::unordered_map < Window::Id, std::weak_ptr<Window>, Window::Id::hasher > windowList;
	};
}