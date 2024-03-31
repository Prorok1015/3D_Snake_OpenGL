#pragma once
#include "../common/common.h"

#include "../render/shader/shader.h"
#include "../render/camera/camera.h"

namespace scene {
	class Model;
}

namespace game 
{
	class GameSystem
	{
	public:
		GameSystem();
		~GameSystem();
		GameSystem(const GameSystem&) = delete;
		GameSystem& operator= (const GameSystem&) = delete;
		GameSystem(GameSystem&&) = default;
		GameSystem& operator= (GameSystem&&) = default;

		void capture();
		void render();

		void begin_frame();
		void end_frame();

		void switch_input(inp::KEYBOARD_BUTTONS code, inp::KEY_ACTION action);
		void set_enable_input(bool enable);

		std::shared_ptr<wnd::Window> get_window() { return window; };
		void load_model(std::string_view path);
		void reload_shaders();

		float cube_scale = 1.f;
		bool is_show_normal = false;
	private:
		std::shared_ptr<scene::Model> ourModel;
		std::shared_ptr<Shader> ourShader;
		std::shared_ptr<Shader> NormalVisualizeShader;
		std::shared_ptr<Camera> camera;

		std::shared_ptr<wnd::Window> window;
		std::shared_ptr<inp::InputManager> input;

		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

		float camX = 0.0f;
		float camY = 0.0f;

	};

	GameSystem& get_system();
}

namespace gm = game;