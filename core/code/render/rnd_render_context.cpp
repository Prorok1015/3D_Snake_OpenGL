#include "rnd_render_context.h"

bool render::RenderContext::init(GLFWwindow* window)
{
	if (is_inited()) {
		return true;
	}

	set_window_id(window);
	set_is_inited(true);

	init_impl();

	return true;
}

void render::RenderContext::swap_buffers()
{
	ASSERT_MSG(is_inited(), "Render Context has had not inited!");
	glfwSwapBuffers(_window_id);
	set_next_frame();
}

void render::RenderContext::init_impl()
{
	/* Make the window's context current */
	glfwMakeContextCurrent(_window_id);
}
