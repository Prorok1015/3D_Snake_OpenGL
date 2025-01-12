#pragma once
#include "wnd_window.h"
#include "rnd_render_contex_interface.h"
#include "gui_backend_interface.h"
#include <string>

namespace wnd
{
	class WindowSystem
	{
	public:
		WindowSystem();
		~WindowSystem();
		WindowSystem(WindowSystem&&) = delete;
		WindowSystem& operator= (WindowSystem&&) = delete;
		WindowSystem(const WindowSystem&) = delete;
		WindowSystem& operator= (const WindowSystem&) = delete;

		std::shared_ptr<window> get_active_window();
		std::shared_ptr<window> find_window(window::short_id win);
		rnd::driver::render_context_interface* get_context() const { return context.get(); }
		gui::imgui_backend_interface* get_gui_backend() const { return imgui_backend.get(); }
		bool is_stop_running();

		void init_windows_frame_time() const;
		void process_windows() const;

	private:
		std::shared_ptr<window> make_window(); 

	private:
		std::unordered_map<window::short_id, std::shared_ptr<window>, window::short_id::hasher> windows_list;
		std::unique_ptr<rnd::driver::render_context_interface> context = nullptr;
		std::unique_ptr<gui::imgui_backend_interface> imgui_backend = nullptr;

		window::short_id active_window;
		wnd::context::header title;
	};
	
	WindowSystem& get_system();
}
