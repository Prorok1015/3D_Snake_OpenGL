#pragma once
#include "../common/common.h"
#include "../resource/res_resource_texture.h"
#include "../render/rnd_render_context.h"

struct GLFWwindow;
enum class CursorMode;

namespace windows
{
	class Window
	{
	public:
		struct Id
		{
			GLFWwindow* id_ = nullptr;

			auto operator<=>(const Id&) const = default;
			std::size_t get_id() const { return std::size_t(id_); }
			operator GLFWwindow* () const { return id_; }
			struct Hasher {
				std::size_t operator() (const Id& id) const { return id.get_id(); }
			};
		};

	public:
		Window(std::string_view title, glm::ivec2 size);
		~Window();
		float aspect_ratio() const { if (size == glm::zero<glm::ivec2>()) return 0.f; return (float)size.x / (float)size.y; }
		bool is_shutdown() const;
		void on_update();
		void shutdown(bool close = true);
		void set_cursor_mode(CursorMode mode);
		void update_frame();

		void on_resize_window(int width, int height);

		void set_logo(res::ImageRef logo, res::ImageRef logo_small);
		void set_vsync(bool v) { glfwSwapInterval(v ? 1 : 0); }

		bool is_minimize_mode() const {
			return size == glm::zero<glm::ivec2>();
		}

		float get_delta() const { return (float)delta; }
		const glm::ivec2& get_size() const { return size; }
		Id get_id() const { return id_; }

	public:
		Event<void(Window& window)> eventRefreshWindow;
		Event<void(Window& window, int width, int height)> eventResizeWindow;

	private:
		Id id_;
		glm::ivec2 size{ 0 };
		double delta = 0.f;
		double lastTime = 0.f;
		rnd::RenderContext _render_context;

	};
}

namespace wnd = windows;