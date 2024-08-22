#pragma once
#include <common.h>

#include <gs_renderer_3d.h>
#include <camera/rnd_camera.h>
#include <wnd_window.h>
#include <scn_camera_controller.h>

namespace scene {
	class Model;
}

namespace gs 
{
	class GameSystem
	{
	public:
		GameSystem();
		~GameSystem();
		GameSystem(const GameSystem&) = delete;
		GameSystem& operator= (const GameSystem&) = delete;
		GameSystem(GameSystem&&) = delete;
		GameSystem& operator= (GameSystem&&) = delete;

		void set_enable_input(bool enable);

		std::shared_ptr<wnd::window> get_window() { return window; };
		void load_model(std::string_view path);
		void reload_shaders();

		void add_cube_to_scene(float radius);
		void remove_cube();

		std::shared_ptr<renderer_3d> get_renderer() const { return renderer; }
		std::shared_ptr<inp::input_manager> get_input_manager() const { return input; }

	private:
		std::shared_ptr<renderer_3d> renderer;

		std::shared_ptr<wnd::window> window;
		std::shared_ptr<inp::input_manager> input;
	};

	GameSystem& get_system();
}