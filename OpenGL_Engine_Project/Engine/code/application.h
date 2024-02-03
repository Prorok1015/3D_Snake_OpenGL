#pragma once
#include "common/common.h"

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

		virtual int run();
	public:
		Event<> beginFrame;
		Event<> capture;
		Event<> render;
		Event<> endFrame;
	};
}

namespace app = application;