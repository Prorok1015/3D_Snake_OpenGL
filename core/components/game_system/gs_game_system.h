#pragma once
#include <common.h>

#include <gs_renderer_3d.h>
#include <camera/scene_camera.h>
#include <wnd_window.h>

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

		scn::Transform get_camera_transform() const { return camera->transform; }

		std::shared_ptr<renderer_3d> get_renderer() const { return renderer; }

		float cube_scale = 1.f;
		bool is_show_normal = false;
	private:
		std::shared_ptr<renderer_3d> renderer;
		std::shared_ptr<camera_static> camera;

		std::shared_ptr<wnd::window> window;
		std::shared_ptr<inp::InputManager> input;

		float camX = 0.0f;
		float camY = 0.0f;

	};

	GameSystem& get_system();
}