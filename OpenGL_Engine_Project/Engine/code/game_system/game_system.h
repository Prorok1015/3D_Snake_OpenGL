#pragma once
#include "../common/common.h"
#include "../windows/display.h"

#include "../render/shader/shader.h"
#include "../render/camera/camera.h"

#include "../debug_ui/debug_ui_init.h"

namespace scene {
	class Model;
}

namespace application {
	class GameSystem
	{
	public:
		GameSystem();
		~GameSystem();

		void capture();
		void render();

		void begin_frame();
		void end_frame();

		std::shared_ptr<Window> get_window() { return display.window; };
	private:
		std::unique_ptr<scene::Model> ourModel;
		std::unique_ptr<Shader> ourShader;
		std::unique_ptr<Camera> camera;

		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;
		Display display;

		float camX = 0.0f;
		float camY = 0.0f;
	};
}

namespace app = application;