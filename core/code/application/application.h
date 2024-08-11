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
		Application(Application&&) = default;
		Application& operator= (Application&&) = default;
		Application(const Application&) = delete;
		Application& operator= (const Application&) = delete;

		virtual int run();

		void set_clear_color(const glm::vec4& clear_color) {
			clear_color_ = clear_color;
		}

		const glm::vec4& get_clear_color() const { return clear_color_; }

	public:
		Event<> beginFrame;
		Event<> capture;
		Event<> prepair_render;
		Event<> pre_render;
		Event<> render;
		Event<> post_render;
		Event<> endFrame;

	private:
		glm::vec4 clear_color_ = { 0.45f, 0.55f, 0.60f, 1.00f };
	};

	Application& get_app_system();
}

namespace app = application;