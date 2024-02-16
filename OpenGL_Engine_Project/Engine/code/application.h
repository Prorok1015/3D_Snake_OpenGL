#pragma once
#include "common/common.h"

namespace application
{
	class Application
	{

	public:
		Application();
		virtual ~Application();
		Application(Application&&) = default;
		Application& operator= (Application&&) = default;
		Application(const Application&) = delete;
		Application& operator= (const Application&) = delete;

		virtual int run();
	public:
		Event<> beginFrame;
		Event<> capture;
		Event<> render;
		Event<> endFrame;
	};

	Application& get_app_system();
}

namespace app = application;