#pragma once
#include <common.h>
#include <glm/glm.hpp>

namespace application
{
	class Application
	{

	public:
		Application();
		virtual ~Application();
		Application(Application&&) = delete;
		Application& operator= (Application&&) = delete;
		Application(const Application&) = delete;
		Application& operator= (const Application&) = delete;

		virtual int run();

	};

	Application& get_app_system();
}

namespace app = application;