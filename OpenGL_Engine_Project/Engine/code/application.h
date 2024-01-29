#pragma once
#include "common/common.h"

#include "render/shader/shader.h"
#include "render/camera/camera.h"
#include "windows/display.h"

namespace scene {
	class Model;
}

namespace application
{
	class Application
	{

	public:
		Application();
		virtual ~Application();
		Application& operator= (const Application&) = default;
		Application& operator= (Application&&) = default;
		Application(const Application&) = default;
		Application(Application&&) = default;

	public:
		static int init_libraris();
		virtual int Run();

	private:
		std::unique_ptr<scene::Model> ourModel;
		std::unique_ptr<Shader> ourShader;
		std::unique_ptr<Camera> camera;

		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;
		Display display;
	};
}

namespace app = application;