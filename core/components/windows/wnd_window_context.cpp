#include "wnd_window_context.h"
#include <GLFW/glfw3.h>

bool window::WindowContext::init(GLFWwindow* window)
{
	if (is_inited()) {
		return true;
	}

	set_window_id(window);
	set_is_inited(true);

	init_impl();

	return true;
}

void window::WindowContext::swap_buffers()
{
	ASSERT_MSG(is_inited(), "Render Context has had not inited!");
	glfwSwapBuffers(_window_id);
	set_next_frame();
}

void window::WindowContext::init_impl()
{
	/* Make the window's context current */
	glfwMakeContextCurrent(_window_id);
}
