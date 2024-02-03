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
		std::weak_ptr<Window> find_window(GLFWwindow* win);
		bool is_all_windows_close();
	private:
		std::unordered_map<GLFWwindow*, std::weak_ptr<Window>> windowList;
	};
}