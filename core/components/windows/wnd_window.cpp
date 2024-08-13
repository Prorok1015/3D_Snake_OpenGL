#include "wnd_window.h"
#include <GLFW/glfw3.h>
#include <enums.h>
#include <timer.hpp>

window::Window::Window(std::string_view title, glm::ivec2 size_)
    : size(size_)
{   
    /* Create a windowed mode window */
    id_.id_ = glfwCreateWindow(size.x, size.y, title.data(), NULL, NULL);

    ASSERT_MSG(id_, "Window didn't create!");

    _render_context.init(id_);
}

window::Window::~Window()
{
    //glfwDestroyWindow
}

void window::Window::set_vsync(bool val)
{
    glfwSwapInterval(val ? 1 : 0);
}

void window::Window::on_update()
{
    _render_context.swap_buffers();
}

bool window::Window::is_shutdown() const
{
    return glfwWindowShouldClose(id_);
}

void window::Window::shutdown(bool close)
{
    glfwSetWindowShouldClose(id_, close);
}

void window::Window::set_cursor_mode(CursorMode mode) {
    glfwSetInputMode(id_, GLFW_CURSOR, (int)mode);
}

void window::Window::update_frame()
{
    double currentTime = Timer::now();
    delta = currentTime - lastTime;
    lastTime = currentTime;
}

void window::Window::on_resize_window(int width, int height)
{
    size = { width, height };
    eventResizeWindow(*this, width, height);
}

void window::Window::set_logo(res::ImageRef logo, res::ImageRef logo_small)
{
    if (logo->channels() != 4) {
        egLOG("window/logo", "Logo should be rgba image");
        return;
    }

    GLFWimage images[2];
    images[0].pixels = logo->data();
    images[0].width = logo->size().x;
    images[0].height = logo->size().y;

    if (logo_small) {
        images[1].pixels = logo_small->data();
        images[1].width = logo_small->size().x;
        images[1].height = logo_small->size().y;
        glfwSetWindowIcon(id_, 2, images);
        return;
    }

    glfwSetWindowIcon(id_, 1, images);
}
